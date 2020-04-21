#ifndef SFT_H
#define SFT_H

#include <QObject>
#include <QVector>
struct {
    float re;
    float im;
    float freq;
    float amp;
    float phase;
}typedef wave;

class SFT : public QObject
{
    Q_OBJECT
public:
    explicit SFT(QObject *parent = nullptr);
    QVector<wave> fourier(QVector<float> ,int freq);
    QVector<QVector<wave>> fourier2D(QVector<QVector<float>> ,int freqx,int freqy);
    QVector<float> reverse(QVector<wave> data);
signals:
private:

};

#endif // SFT_H
