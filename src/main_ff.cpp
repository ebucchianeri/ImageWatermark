#define cimg_display 0
#include <vector>
#include <iostream>
#include <ff/farm.hpp>
#include <chrono>
#include <string>
#include "CImg.h"
#include "util.hpp"


// FF sulla macchina: /usr/local/
//g++ main_ff.cpp -std=c++17 -O3 -pthread -lstdc++fs -o farm.bin -DDEBUG -I /usr/local/

using namespace ff;
using namespace cimg_library;

// WORKER
struct worker: ff_node_t<immagine> {
   CImg<IMGT>* firma;

   worker(CImg<IMGT>* f){
      firma = f;
   }

   immagine *svc(immagine *t){
       applicafirma(t,firma);
       return GO_ON;
   }
};


// EMITTER
struct firstStage: ff_node_t<immagine> {
   vector<immagine*>* v;

   firstStage(vector<immagine*>* f){
      v = f;
   }

  immagine *svc(immagine *t){
    for( auto im : *v ){
      ff_send_out(im);
    }
    return EOS;
  }
};


int main(int argc, char *argv[]){

   if(argc != 7) {
      std::cout << "Parametri : " << std::endl;
      std::cout << "1. " << argv[0] << std::endl;
      std::cout << "2. numero di immagini "<< std::endl;
      std::cout << "3. path della sottocartella contenente immagini "<< std::endl;
      std::cout << "4. nome sottocartella in cui memorizzare i risultati  " << std::endl;
      std::cout << "5. nome del file da utilizzare come firma  " << std::endl;
      std::cout << "6. #threads " << std::endl;
      std::cout << "7. 0 load | 1 copy" << std::endl;
      return(0);
   }

   int nimmagini = atoi(argv[1]);         // numero di immagini che creo e processo
   string path(argv[2]);                  // nome sottocartella
   auto name = argv[4];                   // nome file firma
   string cartella_risultati(argv[3]);    // nome sottocartella dei risultati
   int n = atoi(argv[5]);                 // numero di thread
   int copia = atoi(argv[6]);             // se copia = 1 le immagini sono copiate dalla memoria

   vector<immagine*>* vec = new vector<immagine*>();
	CImg<IMGT> firma(name);                 // immagine firma da passare ai thread
   int nimmaginitotale;                    // numero complessivo delle immagini
	
	// creo la cartella dove mettere le immagini finali
   setup(cartella_risultati);

   float tot_working = 0.0;               // variabile per tempo totale per applicare le firme

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


   // -- APPLICO FIRMA, creo i worker, la farm, rimuovo il collector e aggiungo emitter
   auto T1 = tempi::time::now();
   vector<unique_ptr<ff_node>> workers;
   for(int i=0;i<n;i++){
      workers.push_back(make_unique<worker>(&firma));
   }
   
   ff_Farm<immagine> farm(move(workers));
   farm.remove_collector();
   struct firstStage emitter(vec);
   farm.add_emitter(emitter);

   farm.run_and_wait_end();
   
   auto T2 = tempi::time::now();
   tempi::ms work = std::chrono::duration_cast<tempi::ms>(T2 - T1);
   tot_working = work.count();

   // -- SALVATAGGIO
   salvataggio(vec);
   delete vec;

   std::cout << std::endl;

   std::cout << "Parallelo in FF, con caricamento in memoria delle immagini: " << argv[0] << std::endl;
   std::cout << "Tempo di completamento: " << tot_working << "ms" << std::endl;
   std::cout << "Numero immagini: " << nimmaginitotale << std::endl;
   std::cout << "Numero thread: " << n << std::endl;

   return 0;
}
