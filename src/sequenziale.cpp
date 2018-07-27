#define cimg_display 0
#include <iostream>
#include <chrono>
#include <string>
#include "CImg.h"
#include "util.hpp"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;

//g++ sequenziale.cpp -std=c++17 -O3 -pthread -lstdc++fs -o farm.bin 


int main(int argc, char *argv[]) {

   if(argc != 5) {
      cout << "Parametri : " << endl;
      cout << "1. " << argv[0] << endl;
      cout << "2. numero di immagini "<< endl;
      cout << "3. path della sottocartella contenente le immagini "<< endl;
      cout << "4. nome sottocartella in cui memorizzare i risultati  " << endl;
      cout << "5. nome del file da utilizzare come firma  " << endl;
      return(0);
   }

   int nimmagini = atoi(argv[1]);         // numero di immagini che uso
   string path(argv[2]);                  // nome sottocartella
   auto name = argv[4];                   // nome file firma
   string cartella_risultati(argv[3]);    // nome sottocartella dei risultati

	CImg<IMGT>* img;
	CImg<IMGT> firma(name);           // immagine firma da passare ai thread

   // creo la cartella dove mettere le immagini finali
   setup(cartella_risultati);

   float tot_working = 0.0;        // tempo totale per applicare le firme
   float tot_write_disk = 0.0;     // tempo totale per salvare tutte le immagini
   float tot_read_disk = 0.0;      // tempo totale per caricare tutte le immagini
   float tot = 0.0;                // tempo complessivo 
   
   
   // Apro i file nella cartella e faccio il calcolo, ogni file della cartella
   // lo considero nimmagini volte

   int j = 0;
   for (auto & p : fs::directory_iterator(path)){
      for(int i=0; i<nimmagini; i++){
         // caricamento immagine:
         auto T1 = tempi::time::now();
         immagine* IMG = new immagine();
            #ifdef DEBUG
               cout << "Caricata " << p.path().string() << " per la " << i << endl;
            #endif
         img = new CImg<IMGT>(p.path().string().c_str());
         auto T2 = tempi::time::now();
         // applicazione della firma all' immagine:
         IMG->img = img;
         IMG->nome = nome_file(cartella_risultati+"/"+p.path().filename().string(),i);
         applicafirma(IMG, &firma);
         auto T3 = tempi::time::now();
         // salvataggio dell'immagine
         string path_file(IMG->nome);
         img->save(path_file.c_str());
         delete img;
         delete IMG;
            #ifdef DEBUG
               cout << "Salvata immagine: " << IMG->nome << endl;
            #endif
         j++;
         auto T4 = tempi::time::now();
         tempi::ms work = chrono::duration_cast<tempi::ms>(T3 - T2);
         tempi::ms read = chrono::duration_cast<tempi::ms>(T2 - T1);
         tempi::ms write = chrono::duration_cast<tempi::ms>(T4 - T3);
         tot_working = tot_working + work.count();
         tot_read_disk = tot_read_disk + read.count();
         tot_write_disk = tot_write_disk + write.count();
      }

   }


  tot = tot_working + tot_read_disk + tot_write_disk;

  cout << endl;
  cout << "Sequenziale, senza caricare prima tutte le immagini in memoria: " << argv[0] << endl;
  cout << "Tempo di completamento: " << tot << "ms" << endl;
  cout << "Numero immagini: " << j << endl;
  cout << endl;
  cout << "Tot read disco: " << tot_read_disk << "ms\n";
  cout << "Tot write disco: " << tot_write_disk << "ms\n";
  cout << "Tot lavoro: " << tot_working << "ms\n";
  cout << endl;


	return 0;
}
