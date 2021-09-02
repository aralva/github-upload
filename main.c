#include "mbed.h"
#include "L3G4200D.h"

Timer timer;
Serial pc(USBTX,USBRX);
L3G4200D gyro(D14, D15);

int main(){

const int massa = 80;
const float gyroKerroin = 0.07, g = 9.81, dragAlku = 14.1260177217098, dragMuutos = dragAlku / 90;
float aika = 0, naytevali = 0, nopeus = 0, kiihtyvyys = 0, drag = 0, korkeus = 3000;
int gyroLuku[3] = {0};
float kulma[3] = {0.0};

timer.start();
timer.reset();

    while(true){
        naytevali = timer.read_ms();
        /* Tästä eteenpäin näyteväli muunnetaan aina sekunneiksi */
        aika += naytevali / 1000;
        timer.reset();
		gyro.read(gyroLuku);
		/* Kalibroidaan gyroanturi nollaan halutulla tasolla */
        gyroLuku[0] += 9;
        /* Gyrokerroin on anturin oma lisäkerroin laaduttomalle syötteelle */
        kulma[0] += gyroKerroin * (float)gyroLuku[0] * (naytevali / 1000);
        drag = dragAlku + dragMuutos * kulma[0];
        if(drag < dragAlku){
            drag = dragAlku - dragMuutos * kulma[0];
        }
        if(aika > 40){
            drag = dragAlku * 11;
        }
        kiihtyvyys = (g * massa - drag * nopeus) / massa * (naytevali / 1000);
        nopeus += kiihtyvyys;
        /* Rajanopeus ihmiselle vapaassa pudotuksessa n. 200 km/h */
        if(nopeus >= 200 * 1000 / 60 * 60){
            nopeus = 200 * 1000 / 60 * 60;
        }
        /* Laskuvarjo avataan 40 sekuntia hypyn aloittamisesta */
        korkeus -= nopeus * (naytevali / 1000);
        pc.printf("\nHyppaajan nopeus ja korkeus hetkella %f sekuntia: \n\nNopeus : %.2f m/s\n\nKorkeus: %f.4 m\n", aika, nopeus, korkeus);
    }
}