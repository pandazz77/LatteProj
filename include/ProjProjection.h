#pragma once

#include <Latte/Projection/IProjection.hpp>
#include <proj/coordinateoperation.hpp>
#include <proj/crs.hpp>
#include <proj/io.hpp>

class ProjProjection: public IProjection{
    public:
        ProjProjection(osgeo::proj::crs::CRSNNPtr targetCRS);

        static ProjProjection fromEPSG(QString code);

        QPointF project(const LatLng &latlng) const override;
        LatLng unproject(const QPointF &point) const override;
        Bounds bounds() const override;

    private:
        static PJ_COORD PJ_COORD_2D(double v1, double v2);
        PJ_COORD transform(PJ_COORD coord, bool forward) const;
        
    private:
        osgeo::proj::crs::CRSNNPtr _targetCRS;
        osgeo::proj::operation::CoordinateOperationPtr _forwardOP;
        osgeo::proj::operation::CoordinateOperationPtr _backwardOP;
        Bounds _bounds;


        static osgeo::proj::io::DatabaseContextNNPtr dbContext;
        static osgeo::proj::io::AuthorityFactoryNNPtr authFactory;
        static osgeo::proj::operation::CoordinateOperationContextNNPtr coord_op_ctxt;
        static osgeo::proj::io::AuthorityFactoryNNPtr authFactoryEPSG;
        static osgeo::proj::crs::CRSNNPtr sourceCRS;

        
};