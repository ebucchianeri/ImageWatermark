#define cimg_display 0
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

#include "CImg.h"
#include "util.hpp"
#include "queue.hpp"

using namespace cimg_library;

// funzione che ogni thread esegue
void func(CImg<IMGT>* firma, queue<immagine*>& input) {
   immagine* current = NULL;
   int i=0;
   bool continua = true;
    while(continua){
      current=input.pop();
      if (current!=NULL){
         applicafirma(current, firma);
         i++;
	    } else {
         #ifdef DEBUG
	       std::cout << "- THREAD TERMINO. Calcolate #immagini:" << i << std::endl;
         #endif
         continua = false;
	    }
   }
}



int main(int argc, char *argv[]) {

   if(argc != 7) {
      std::cout << "Parametri : " << std::endl;
      std::cout << "1. " << argv[0] << std::endl;
      std::cout << "2. numero di immagini "<< std::endl;
      std::cout << "3. path della sottocartella delle immagini "<< std::endl;
      std::cout << "4. nome sottocartella in cui memorizzare i risultati  " << std::endl;
      std::cout << "5. nome del file da utilizzare come firma  " << std::endl;
      std::cout << "6. #thread " << std::endl;
      std::cout << "7. 0 load | 1 copy" << std::endl;
      return(0);
   }


   int nimmagini = atoi(argv[1]);            // numero di immagini da considerare
   string path(argv[2]);                     // nome sottocartella
   string cartella_risultati(argv[3]);       // nome sottocartella dei risultati
   auto name = argv[4];                      // nome file firma
   int n = atoi(argv[5]);                    // numero di thread
   int copia = atoi(argv[6]);                // se copia = 1 le immagini sono copiate dalla memoria

   vector<immagine*>* vec = new vector<immagine*>();  // vettore di immagini
   queue<immagine*> input;                   // vettore di code di input, una per thread
	vector<thread> threads;                   // vettore di thread
   CImg<IMGT> firma(name);                   // immagine firma da passare ai thread
   int nimmaginitotale;

   float tot_working = 0.0;                  // variabile per tempo totale per applicare le firme

   // se la cartella_risultati non esiste la creo.
   setup(cartella_risultati);

   // -- CARICAMENTO
   if(copia == 0)
      caricamento(nimmagini,vec,path,cartella_risultati);
   else if(copia == 1)
      caricamento_copia(nimmagini,vec,path,cartella_risultati);
   else {  
      std::cout << "Errore parametri" << std::endl;
      return -1;
   }
   nimmaginitotale = vec->size();


   // -- APPLICO LA FIRMA e genero i worker
   auto T1 = tempi::time::now();
   for(int i=0;i<n;i++)
      threads.push_back(thread(func,&firma,ref(input)));

   for(int i = 0; i<vec->size(); i++)
      input.push(vec->at(i));

   for(int i=0;i<n;i++)
	   input.push(NULL);


	for(int i=0;i<n;i++)
	   threads[i].join();

   auto T2 = tempi::time::now();
   tempi::ms work = std::chrono::duration_cast<tempi::ms>(T2 - T1);
   tot_working = work.count();

   // -- SALVO:
   salvataggio(vec);
   delete vec;

   std::cout << std::endl;
   std::cout << "Parallelo con thread C++, con caricamento in memoria delle immagini: " << argv[0] << std::endl;
   std::cout << "Tempo di completamento: " << tot_working << "ms" << std::endl;
   std::cout << "Numero immagini: " << nimmaginitotale << std::endl;
   std::cout << "Numero thread: " << n << std::endl;
   return 0;
}
