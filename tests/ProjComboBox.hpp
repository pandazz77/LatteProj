#pragma once

#include "IProjection.hpp"
#include "SimpleProjection.h"
#include "Mercator.h"
#include "SphericalMercator.h"
#include "MapGraphicsView.h"

#include "ProjProjection.h"
#include <QComboBox>

class ProjectionsFabric{
    public:
        template<typename Proj>
        void regProjection(QString projName){
            factoryMap[projName] = [](){ return new Proj(); };
        }

        void regProjection(QString projName, std::function<IProjection*()> fabric){
            factoryMap[projName] = fabric;
        }

        IProjection *get(QString projName) const{
            return factoryMap[projName]();
        }

        QStringList projections() const {
            return factoryMap.keys();
        }

    private:
        QMap<QString,std::function<IProjection*()>> factoryMap;
};

class ProjComboBox: public QComboBox{
    public:
        ProjComboBox(MapGraphicsView *view) : QComboBox(view), view(view) {
            projFabric.regProjection<SimpleProjection>("Simple projection");
            projFabric.regProjection<Mercator>("Mercator");
            projFabric.regProjection<SphericalMercator>("Spherical mercator");

            QComboBox::addItems(projFabric.projections());
            QComboBox::setCurrentText("Simple projection");

            connect(this,&QComboBox::currentTextChanged,[this](QString projName){
                if(projName.isEmpty()) return;
                if(this->view) 
                    this->view->setProjection(projFabric.get(projName));
            });

        }

        ProjectionsFabric &fabric() {
            return projFabric;
        }

        void update(){
            QString save = currentText();
            QComboBox::blockSignals(true);
            QComboBox::clear();
            QComboBox::addItems(projFabric.projections());
            QComboBox::blockSignals(false);
            if(currentText()!=save){
                if(projFabric.projections().contains(save))
                    setCurrentText(save);
            }
        }

        void setView(MapGraphicsView *view){
            this->view = view;
        }

    private:
        ProjectionsFabric projFabric;
        MapGraphicsView *view;
};