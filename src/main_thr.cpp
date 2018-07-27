#define cimg_display 0
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

#include "CImg.h"
#include "util.hpp"
#include "queue_attiva.hpp"

using namespace cimg_library;
using namespace std;

// g++ main.thr.cpp -std=c++17 -O3 -pthread -lstdc++fs

// funzione che ogni thread eseegue
void func(CImg<IMGT>* firma, int nt, queue_q<immagine*>& input) {
   immagine* current = NULL;
   CImg<IMGT>* currentimg = NULL;
   int i=0;
   bool continua = true;
    while(continua){
      current=input.pop();
      if (current!=NULL){
         applicafirma(current, firma);
         i++;
	    } else {
	      //cout << "Thread " << nt << " termina, calcolate  " << i << " immagini." << endl;
         continua = false;
	    }
   }
}





int main(int argc, char *argv[]) {

   if(argc != 7) {
      cout << "Parametri : " << endl;
      cout << "1. " << argv[0] << endl;
      cout << "2. numero di immagini "<< endl;
      cout << "3. path della sottocartella delle immagini "<< endl;
      cout << "4. nome sottocartella in cui memorizzare i risultati  " << endl;
      cout << "5. nome del file da utilizzare come firma  " << endl;
      cout << "6. #thread " << std::endl;
      cout << "7. 0 load | 1 copy " << std::endl;
      return(0);
   }

   int nimmagini = atoi(argv[1]);            // numero di immagini da considerare
   string path(argv[2]);                     // nome sottocartella
   string cartella_risultati(argv[3]);       // nome sottocartella dei risultati
   auto name = argv[4];                      // nome file firma
   int n = atoi(argv[5]);                    // numero di thread
   int copia = atoi(argv[6]);                // se copia = 1 le immagini sono copiate dalla memoria

   vector<immagine*>* vec = new vector<immagine*>();  // vettore di immagini
   vector<queue_q<immagine*>> input(n);        // vettore di code di input, una per thread
	vector<thread> threads;                     // vettore di thread
   CImg<IMGT> firma(name);                     // immagine firma da passare ai thread
   int nimmaginitotale; 

   float tot_working = 0.0;                    // variabile per calcolare tempo totale per applicare le firme

   // se la cartella_risultati non esiste la creo.
   setup(cartella_risultati);


   // ---- CARICO I FILE:
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



   // ----- CALCOLO: La computazione comincia da qua.
   auto T1 = tempi::time::now();
   // creo gli n thread. Distribuisco in RR
   for(int i=0;i<n;i++){
      threads.push_back(thread(func,&firma,i,ref(input[i])));
   }
   
   for(int i = 0; i<vec->size(); i++){
      int index = i%n;
      input[index].push(vec->at(i));
   }


   // faccio terminare i threads aggiungendo n NULL in CODA e attendo la fine dei thread
   for(int i=0;i<n;i++)
      input[i].push(NULL);

   for(int i=0;i<n;i++)
      threads[i].join();

   auto T2 = tempi::time::now();
   tempi::ms work = std::chrono::duration_cast<tempi::ms>(T2 - T1);
   tot_working = work.count();


   // ---- SALVO:
   salvataggio(vec);
   delete vec;



   cout << endl;

   cout << "Parallelo con thread C++, con caricamento in memoria delle immagini e RR: " << argv[0] << endl;
   cout << "Tempo di completamento: " << tot_working << "ms" << endl;
   cout << "Numero immagini: " << nimmaginitotale << endl;
   cout << "Numero thread: " << n << endl;
   

   return 0;
}
