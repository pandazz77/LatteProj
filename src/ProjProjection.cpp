#include "ProjProjection.h"


#include <proj/util.hpp> // for nn_dynamic_pointer_cast
#include <cmath>   // for HUGE_VAL

using namespace NS_PROJ::crs;
using namespace NS_PROJ::io;
using namespace NS_PROJ::operation;
using namespace NS_PROJ::util;

DatabaseContextNNPtr ProjProjection::dbContext = DatabaseContext::create();
AuthorityFactoryNNPtr ProjProjection::authFactory = AuthorityFactory::create(dbContext,std::string());
CoordinateOperationContextNNPtr ProjProjection::coord_op_ctxt = CoordinateOperationContext::create(authFactory,nullptr,0.0);
AuthorityFactoryNNPtr ProjProjection::authFactoryEPSG = AuthorityFactory::create(dbContext,"EPSG");
CRSNNPtr ProjProjection::sourceCRS = authFactoryEPSG->createCoordinateReferenceSystem("4326");

ProjProjection::ProjProjection(CRSNNPtr targetCRS) : _targetCRS(targetCRS), _bounds({},{}){
    // _operations = CoordinateOperationFactory::create()->createOperations(sourceCRS,_targetCRS,coord_op_ctxt);
    // assert(!_operations.empty());
    _forwardOP = CoordinateOperationFactory::create()->createOperation(sourceCRS,_targetCRS);
    _backwardOP = CoordinateOperationFactory::create()->createOperation(_targetCRS, sourceCRS);

    PJ_CONTEXT *ctx = proj_context_create();
    double Wlng, Slat, Elng, Nlat;

    std::string epsg = "EPSG:"+std::to_string(_targetCRS->getEPSGCode());
    PJ *crs = proj_create(ctx,epsg.c_str());
    proj_get_area_of_use(ctx,crs,&Wlng,&Slat,&Elng,&Nlat,nullptr);
    proj_context_destroy(ctx);
    proj_destroy(crs);

    _bounds.northEast = {Nlat,Elng};
    _bounds.southWest = {Slat,Wlng};
}

ProjProjection ProjProjection::fromEPSG(QString code){
    CRSNNPtr target = authFactoryEPSG->createCoordinateReferenceSystem(code.toStdString());
    return ProjProjection(target);
}

PJ_COORD ProjProjection::PJ_COORD_2D(double v1, double v2){
    return PJ_COORD{{
        v1,
        v2,
        0.0, // z ordinate. unused
        HUGE_VAL // time ordinate. unused
    }};
}

PJ_COORD ProjProjection::transform(PJ_COORD coord, bool forward) const {
    PJ_CONTEXT *ctx = proj_context_create();
    CoordinateOperationPtr operation = forward ? _forwardOP : _backwardOP;
    auto transformer = operation->coordinateTransformer(ctx);

    coord = transformer->transform(coord);
    proj_context_destroy(ctx);

    return coord;
}

QPointF ProjProjection::project(const LatLng &latlng) const{
    PJ_COORD projected = transform(
        PJ_COORD_2D(latlng.lat(),latlng.lng()),
        true
    );
    return QPointF{
        projected.v[0],
        projected.v[1]
    };
}

LatLng ProjProjection::unproject(const QPointF &point) const{
    PJ_COORD unprojected = transform(
        PJ_COORD_2D(point.x(),point.y()),
        false
    );
    return LatLng(
        unprojected.v[0],
        unprojected.v[1]
    );
}

Bounds ProjProjection::bounds() const{
    return _bounds;
}
