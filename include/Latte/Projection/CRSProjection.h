#pragma once

#include <Latte/Projection/IProjection.hpp>
#include <proj/coordinateoperation.hpp>
#include <proj/crs.hpp>
#include <proj/io.hpp>

class CRSProjection: public IProjection{
    public:
        CRSProjection(osgeo::proj::crs::CRSNNPtr targetCRS);

        static CRSProjection fromEPSG(int epsgCode);

        QPointF project(const LatLng &latlng) const override;
        LatLng unproject(const QPointF &point) const override;
        Bounds bounds() const override;

    private:
        PJ_COORD transform(double ord1, double ord2, bool forward) const;
        
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