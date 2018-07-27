#define cimg_display 0
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "CImg.h"
#include "util.hpp"

using namespace cimg_library;
using namespace std;

// g++ main.seq.cpp -std=c++17 -O3 -pthread -lstdc++fs

int main(int argc, char *argv[]) {

   if(argc != 6) {
      cout << "Parametri : " << endl;
      cout << "1. " << argv[0] << endl;
      cout << "2. numero di immagini "<< endl;
      cout << "3. path della sottocartella contenente le immagini "<< endl;
      cout << "4. nome sottocartella in cui memorizzare i risultati  " << endl;
      cout << "5. nome del file da utilizzare come firma  " << endl;
      cout << "6. 0 load | 1 copy" << endl;
      return(0);
   }

   int nimmagini = atoi(argv[1]);         // numero di immagini che creo e processo
   string path(argv[2]);                  // nome sottocartella
   auto name = argv[4];                   // nome file firma
   string cartella_risultati(argv[3]);    // nome sottocartella dei risultati

   vector<immagine*>* vec = new vector<immagine*>();     // vettore delle immagini
	CImg<IMGT> firma(name);                 // immagine firma da passare ai thread
   int nimmaginitotale;                    // numero complessivo delle immagini
   int copia = atoi(argv[5]);                // se copia = 1 le immagini sono copiate dalla memoria

	// creo la cartella dove mettere le immagini finali
   setup(cartella_risultati);

   float tot_working = 0.0;               // variabile per tempo totale per applicare le firme

   // ---- CARICO I FILE:
   // uso la funzione caricamento per caricare le immagini nel vettore vec
   if(copia == 0)
      caricamento(nimmagini,vec,path,cartella_risultati);
   else if(copia == 1)
      caricamento_copia(nimmagini,vec,path,cartella_risultati);
   else {  
      std::cout << "Errore parametri" << std::endl;
      return -1;
   }
   nimmaginitotale = vec->size();


   // ----- CALCOLO.
   // per ogni immagine nel vettore applico la funzione applicafirma
   auto T1 = tempi::time::now();
   for (auto immagine : *vec)
      applicafirma(immagine, &firma);
   auto T2 = tempi::time::now();
   tempi::ms work = std::chrono::duration_cast<tempi::ms>(T2 - T1);
   tot_working = work.count();


   // ---- SALVO:
   salvataggio(vec);
   delete vec;

   cout << endl;

   cout << "Sequenziale, con caricamento in memoria delle immagini: " << argv[0] << endl;
   cout << "Tempo di completamento: " << tot_working << "ms" << endl;
   cout << "Numero immagini: " << nimmaginitotale << endl;

  return 0;
}
