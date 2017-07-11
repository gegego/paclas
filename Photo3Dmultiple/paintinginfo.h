#ifndef PAINTINGINFO_H
#define PAINTINGINFO_H
#include <QString>

class paintingInfo
{
public:
    paintingInfo(QString path, double height, double width);
    ~paintingInfo();

    QString getPath(){return path;}
    double getHeight(){return height;}
    double getWidth(){return width;}
private:
    double width;
    double height;
    QString path;
};

#endif // PAINTINGINFO_H
