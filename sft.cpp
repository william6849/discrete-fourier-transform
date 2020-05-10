#include "sft.h"
#include <QtMath>
#include <QDebug>
SFT::SFT(QObject *parent) : QObject(parent)
{

}
QVector<wave> SFT::fourier(QVector<float> data,int freq)
{
    qDebug()<<data;
    int x_axis = data.size();
    QVector<wave> result;
    for(int N=0;N<freq;N++)//Cn的n 頻率
    {
        qreal re=0;
        qreal im=0;

        //**********計算積分，TIME間隔為1/AXIS
        for(int t=0;t<x_axis;t++)
        {
            float rate = (1*M_PI_2*(float)N);
            float time = ((float)t/(float)x_axis);//走了總步數的比例，就是走了圓周的多少百分比
            float expvar = (time * rate);

            re+= (data.at(t)*qCos(expvar));
            im+= (data.at(t)*qSin(expvar));
        }
        if (qAbs(re) < 1e-10) { re = 0; }
        if (qAbs(im) < 1e-10) { im = 0; }
        re/=x_axis;
        im/=x_axis;
        //**************
        wave wv;
        wv.re=re;
        wv.im=im;
        wv.freq=N;
        wv.amp=qSqrt(re*re+im*im);
        wv.phase=qAtan2(im, re) * 180 / M_PI;
        result.push_back(wv);

    }
    //for(auto w:result)
   // qDebug()<<"re:"<<w.re<<" im:"<<w.im<<" freq:"<<w.freq<<" amp:"<<w.amp<<" phase:"<<w.phase;
    return result;
}
QVector<QVector<wave>> SFT::fourier2D(QVector<QVector<float>> data,int freqx,int freqy)
{
    qDebug()<<data;
    int x_axis = data.at(0).size();
    int y_axis = data.size();
    QVector<QVector<wave>> result;
    QVector<wave> temp;
    qreal re[x_axis][y_axis];
    qreal im[x_axis][y_axis];
    for(int fy=0;fy<freqy;fy++)//Cn的n 頻率
    {
        for(int fx=0;fx<freqx;fx++)
        {
            for(int y=0;y<y_axis;y++)
            {
                for(int x=0;x<x_axis;x++)
                {
                    float rate = (-1*M_PI_2);
                    float timex = ((float)x/(float)x_axis)*(float)fx;//走了總步數的比例，就是走了圓周的多少百分比
                    float timey = ((float)y/(float)y_axis)*(float)fy;
                    float expvar = ((timex+timey) * rate);

                    re[fy][fx]+= (data.at(y).at(x)*qCos(expvar));
                    im[fy][fx]+= (data.at(y).at(x)*qSin(expvar));
                }
            }
            //            if (qAbs(re) < 1e-10) { re = 0; }
            //            if (qAbs(im) < 1e-10) { im = 0; }
            re[fy][fx]/=x_axis*y_axis;
            im[fy][fx]/=x_axis*y_axis;
            //**************

            wave wv;
            wv.re=re[fy][fx];
            wv.im=im[fy][fx];
            wv.amp=qSqrt(re[fy][fx]*re[fy][fx]+im[fy][fx]*im[fy][fx]);
            wv.phase=qAtan2(im[fy][fx], re[fy][fx]) * 180 / M_PI;
            temp.push_back(wv);
        }
        result.push_back(temp);
        if(!temp.empty())temp.clear();
    }
    return result;

}
QVector<float> SFT::reverse(QVector<wave> data)
{
    QVector<float> result;
    for(int time=0;time<180;time++){
        float intensity=0;
        for(auto w:data)
        {
            //intensity+=w.amp*(w.re*qCos(M_PI_2*w.freq*(float)time/180) - w.im*qSin(M_PI_2*w.freq*(float)time/180));
            intensity+=w.amp*qCos(1*M_PI_2*w.freq*(float)time/180+qAtan2(w.im, w.re));
        }

        result.push_back(intensity);
    }
    qDebug()<<result;
    return result;

}
