#include "Latte/Projection/CRSProjection.h"

#include <QDebug>

PJ_COORD latlng2pj(const LatLng &latlng){
    return proj_coord(latlng.lng(),latlng.lat(),0,0);
}

LatLng pj2latlng(const PJ_COORD &pj){
    return LatLng(pj.v[1],pj.v[0]);
}

PJ_COORD qpoint2pj(const QPointF &point){
    return proj_coord(point.x(),point.y(),0,0);
}

QPointF pj2qpoint(const PJ_COORD &pj){
    return QPointF{pj.v[0], pj.v[1]};
}

const QVector<PJ_TYPE> UNSUPPORTED_PJ_TYPES{
    PJ_TYPE_GEOCENTRIC_CRS,
    PJ_TYPE_GEOGRAPHIC_3D_CRS,
    PJ_TYPE_VERTICAL_CRS,
    PJ_TYPE_COMPOUND_CRS
};

CRSProjection::CRSProjection(PJ *transformation, PJ_CONTEXT *ctx) : transformation(transformation), ctx(ctx){
    PJ *target = targetCRS();

    PJ_TYPE type = proj_get_type(target);
    if(UNSUPPORTED_PJ_TYPES.contains(type)){
        qWarning() << name() << "You are trying to use unsupported PJ_TYPE" << type;
    }

    // extract bbox ============================
    double Wlng, Slat, Elng, Nlat;

    proj_get_area_of_use(ctx,target,&Wlng,&Slat,&Elng,&Nlat,nullptr);

    _bounds.northEast = {Nlat,Elng};
    _bounds.southWest = {Slat,Wlng};
    // ==========================================
}

CRSProjection::CRSProjection(CRSProjection &src){
    this->ctx = proj_context_clone(src.ctx);
    this->transformation = proj_clone(ctx,src.transformation);
    this->_bounds = src._bounds;
}

CRSProjection::CRSProjection(CRSProjection &&src){
    this->ctx = src.ctx;
    src.ctx = nullptr;
    this->transformation = src.transformation;
    src.transformation = nullptr;
    this->_bounds = src._bounds;
}

CRSProjection::~CRSProjection(){
    proj_context_destroy(ctx);
    proj_destroy(transformation);
}

PJ *CRSProjection::targetCRS() const {
    return proj_get_target_crs(ctx,transformation);
}

QString CRSProjection::name() const {
    return proj_get_name(targetCRS());
}

CRSProjection CRSProjection::fromEPSG(int code){
    PJ_CONTEXT *ctx = proj_context_create();
    std::string target_auth = "EPSG:"+std::to_string(code);

    // create transformation
    PJ *P = proj_create_crs_to_crs(ctx, "EPSG:4326", target_auth.c_str(), nullptr);
    if (!P) {
        throw std::runtime_error("Cannot create transformation");
    }

    // Normalize for consistent axis order (lat, lon input)
    PJ* P_norm = proj_normalize_for_visualization(ctx, P);
    if (P_norm) {
        proj_destroy(P);
        P = P_norm;
    }
    return CRSProjection(P,ctx);
}

QPointF CRSProjection::project(const LatLng &latlng) const{
    PJ_COORD projected = proj_trans(transformation, PJ_FWD, latlng2pj(latlng));
    return pj2qpoint(projected);
}

LatLng CRSProjection::unproject(const QPointF &point) const{
    PJ_COORD unprojected = proj_trans(transformation, PJ_INV, qpoint2pj(point));
    return pj2latlng(unprojected);
}

Bounds CRSProjection::bounds() const{
    return _bounds;
}
