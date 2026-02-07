#pragma once

#include <Latte/Projection/IProjection.hpp>
#include <proj.h>

class CRSProjection: public IProjection{
    public:
        static CRSProjection fromEPSG(int epsgCode);
        ~CRSProjection();

        QPointF project(const LatLng &latlng) const override;
        LatLng unproject(const QPointF &point) const override;
        Bounds bounds() const override;

        QString name() const;

    protected:
        CRSProjection(PJ *transformation, PJ_CONTEXT *ctx);

        PJ *targetCRS() const;
        
    private:
        PJ_CONTEXT *ctx = nullptr;
        PJ *transformation = nullptr;
        Bounds _bounds;
        
};