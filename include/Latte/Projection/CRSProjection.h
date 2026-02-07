#pragma once

#include <Latte/Projection/IProjection.hpp>
#include <proj.h>

class CRSProjection: public IProjection{
    public:
        CRSProjection(CRSProjection &src);
        CRSProjection(CRSProjection &&src);

        /**
         * @brief Create CRS projection from string
         * 
         * @param string CRS string. Can be:
            - a "AUTHORITY:CODE", like EPSG:25832
            - a PROJ string, like "+proj=longlat +datum=WGS84".
            - the name of a CRS as found in the PROJ database, e.g "WGS84", "NAD27", etc.
            - more generally any string accepted by proj_create() representing a CRS
         * @return CRSProjection 
         */
        static CRSProjection fromString(QString string);
        /**
         * @brief Create CRS projection from EPSG code
         * 
         * @param epsgCode epsg code
         * @return CRSProjection 
         */
        static CRSProjection fromEPSG(int epsgCode);
        ~CRSProjection();

        QPointF project(const LatLng &latlng) const override;
        LatLng unproject(const QPointF &point) const override;
        Bounds bounds() const override;

        /**
         * @brief Get CRS name
         * 
         * @return QString name of crs
         */
        QString name() const;

    protected:
        CRSProjection(PJ *transformation, PJ_CONTEXT *ctx);

        PJ *targetCRS() const;
        
    private:
        PJ_CONTEXT *ctx = nullptr;
        PJ *transformation = nullptr;
        Bounds _bounds = {{},{}};
        
};