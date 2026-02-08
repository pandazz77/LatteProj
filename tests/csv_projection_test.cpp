#include <QStringList>
#include <QFile>
#include <QDebug>
#include "Latte/Projection/CRSProjection.h"

struct TestCase{
    QString crs_str;
    LatLng wgs84pos;
    QPointF projected;

    TestCase(QString crs_str, LatLng wgs84pos, QPointF projected)
    : crs_str(crs_str), wgs84pos(wgs84pos), projected(projected) {}

    static TestCase fromString(QString string){
        QStringList args = string.split(",");
        return TestCase{
            args[0],
            {
                args[1].toDouble(),
                args[2].toDouble()
            },
            {
                args[3].toDouble(),
                args[4].toDouble()
            }
        };
    }
};

QVector<TestCase> parseCSV(QString csvPath){
    QVector<TestCase> result;
    QFile file(csvPath);
    bool ok_read = file.open(QFile::ReadOnly);
    assert(ok_read);

    for(QString line: file.readAll().split('\n')){
        result.push_back(TestCase::fromString(line));
    }
    return result;
}

void ASSERT_EQUAL(const QPointF &a, const QPointF &b, double delta = 6e-9){
    double xdif = std::abs(a.x() - b.x());
    double ydif = std::abs(a.y() - b.y());
    assert(xdif <= delta && ydif <= delta);
}

void ASSERT_EQUAL(const LatLng &a, const LatLng &b, double delta = 6e-9){
    double latdif = std::abs(a.lat() - b.lat());
    double lngdif = std::abs(a.lng() - b.lng());
    assert(latdif <= delta && lngdif <= delta);
}

void run_test(const TestCase &test){
    CRSProjection crs = CRSProjection::fromString(test.crs_str);
    
    QPointF projected = crs.project(test.wgs84pos);
    ASSERT_EQUAL(projected,test.projected);
    LatLng unprojected = crs.unproject(projected);
    ASSERT_EQUAL(unprojected,test.wgs84pos);
}



int main(int argc, char *argv[]){
    QString exec_name = argv[0];
    QString file = argv[1];

    const QVector<TestCase> tests = parseCSV(file);
    qDebug() << "Count of test cases:" << tests.size();
    int i = 1;
    for(const TestCase &test: tests){
        qDebug().nospace() << "Running tests №" << i++;
        run_test(test);
    }

    qDebug() << "All tests passed!";
    return 0;
}