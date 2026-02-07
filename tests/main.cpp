#include <QApplication>
#include <QCommandLineParser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

#include <Latte/Graphics/View/MapGraphicsView.h>
#include <Latte/Providers/GeoJsonProvider.h>

#include "Latte/Projection/CRSProjection.h"

#include "ProjComboBox.hpp"

GeoJsonProvider provider;

void onGeoJsonOpen(MapGraphicsView *view, QString fileName){
    view->scene()->clear();
    GraphicsGroup *group = provider.fromFile(fileName);
    group->addTo(view);
}

std::function<CRSProjection*()> EPSGfactory(int code){
    return [code](){
        return new CRSProjection(std::move(CRSProjection::fromEPSG(code)));
    };
}

void RegisterEPSG(ProjComboBox *combo, QString name, int code){
    combo->fabric().regProjection(name,EPSGfactory(code));
}


int main(int argc, char *argv[]){
    QApplication app(argc,argv);

    QCommandLineParser parser;
    QCommandLineOption input_file_option(QStringList() << "i" << "input","geojson input file","input");
    parser.addOption(input_file_option);

    parser.process(app);

    QWidget *window = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(window);
    window->resize(800,600);
    window->setLayout(mainLayout);
    QVBoxLayout *rightLayout = new QVBoxLayout(nullptr);

    MapGraphicsView *map = new MapGraphicsView();
    mainLayout->addWidget(map);
    mainLayout->addLayout(rightLayout);

    ProjComboBox *projCombo = new ProjComboBox(map);
    RegisterEPSG(projCombo,"EPSG:3857",3857);
    RegisterEPSG(projCombo,"EPSG:9475",9475);
    RegisterEPSG(projCombo, "EPSG:27700", 27700);
    RegisterEPSG(projCombo,"EPSG:4890",4890);
    projCombo->update();
    rightLayout->addWidget(projCombo);


    QPushButton *selectFileBtn = new QPushButton("Select geojson file",window);
    rightLayout->addWidget(selectFileBtn);
    rightLayout->addStretch(1);
    selectFileBtn->connect(selectFileBtn,&QPushButton::clicked,[&](){
        QString fileName = QFileDialog::getOpenFileName(window,"Open GeoJson file");
        if(fileName.isEmpty()) return;
        onGeoJsonOpen(map,fileName);
    });

    if(parser.isSet(input_file_option)){
        QString filename = parser.value(input_file_option);
        onGeoJsonOpen(map,filename);
    }

    window->show();
    return app.exec(); 
}