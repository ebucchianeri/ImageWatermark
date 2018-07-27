#define cimg_display 0
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include "CImg.h"
#include <experimental/filesystem>


namespace fs = std::experimental::filesystem;
using namespace cimg_library;
using namespace std;


// Tipo delle immagini
typedef unsigned char IMGT;


namespace tempi {
   typedef std::chrono::high_resolution_clock  time;
   typedef std::chrono::milliseconds           ms;
   typedef std::chrono::microseconds           micros;
   typedef std::chrono::duration<float>        fsec;

}




// CLASS IMMAGINE: rappresenta una immagine
class immagine{
   public:
      CImg<IMGT>* img;
      string nome;      // nome con cui l'immagine sara' salvata

      immagine() {}
};

// funzione ausiliaria per generare i nomi delle immagini da salvare
string nome_file(string nome, int n){
      nome.insert(nome.length()-4,"_"+to_string(n));
      return nome;
}




// funzione usata per generare, se inesistente, la cartella in cui andare a salvare le immagini finali
void setup(string cartella_risultati){
   fs::path path_risultati(cartella_risultati);
   if (!fs::is_directory(path_risultati) || !fs::exists(path_risultati)) {
      fs::create_directory(path_risultati);
      std::cout << "Creata cartella " << path_risultati << "." << std::endl;
   }
   std::cout << "Le immagini saranno salvate in " << path_risultati << std::endl;
}




// funzione per inizializzare in vettore di immagini vec passato come parametro.
// parametri di input:
//    nimmagini: ogni immagine nella cartella 'path' viene caricata nimmagini volte
//    vec: puntatore al vettore delle immagini
//    path: path della cartella che contiene le immagini da utilizzare
//    cartella_risultati: nome della cartella in cui andare a salvare le immagini
//             usato per generare subito il nome finale dell'immagine con il metodo 'nome_file' e memorizzarlo
//             nel campo nome dell'oggetto immagine.
void caricamento(int nimmagini, vector<immagine*>* vec, fs::path path, string cartella_risultati){
   for (auto & p : fs::directory_iterator(path)){
      for(int i=0; i<nimmagini; i++){
         immagine* IMG = new immagine();
         CImg<IMGT>* img = new CImg<IMGT>(p.path().string().c_str());
         IMG->img = img;
         IMG->nome = nome_file(cartella_risultati+"/"+p.path().filename().string(),i);
         vec->push_back(IMG);
         #ifdef DEBUG
            std::cout << "Caricata " << p.path().string() << " per la " << i << " volta"<< std::endl;
         #endif
      }
   }
   #ifdef DEBUG
   std::cout << "Caricate " << vec->size() << " immagini" << std::endl;
   #endif
}




// funzione per inizializzare in vettore di immagini vec passato come parametro.
// parametri di input:
//    nimmagini: ogni immagine nella cartella 'path' viene COPIATA nimmagini volte. A differenza della funzione caricamento, ogni immagine nella cartella e' caricata dal disco in memoria una sola volta e poi copiata nimmagini volte.
//    vec: puntatore al vettore delle immagini
//    path: path della cartella che contiene le immagini da utilizzare
//    cartella_risultati: nome della cartella in cui andare a salvare le immagini
//             usato per generare subito il nome finale dell'immagine con il metodo 'nome_file' e memorizzarlo nel campo nome dell'oggetto immagine
void caricamento_copia(int nimmagini, vector<immagine*>* vec, fs::path path, string cartella_risultati){
   for (auto & p : fs::directory_iterator(path)){
      CImg<int> campione(p.path().string().c_str());
      for(int i=0; i<nimmagini; i++){
         immagine* IMG = new immagine();
         CImg<IMGT>* img = new CImg<IMGT>(campione);
         IMG->img = img;
         IMG->nome = nome_file(cartella_risultati+"/"+p.path().filename().string(),i);
         vec->push_back(IMG);
         #ifdef DEBUG
            std::cout << "Caricata " << p.path().string() << " per la " << i << " volta"<< std::endl;
         #endif
      }
   }
   #ifdef DEBUG
   std::cout << "Caricate " << vec->size() << " immagini" << std::endl;
   #endif
}





// funzione per salvare tutte le immagini presenti nel vettore passato come parametro.
// dopo il salvataggio la memoria e' liberata
void salvataggio(vector<immagine*>* vec){
   for (auto current : *vec){
      CImg<IMGT>* currentimg = current->img;
      currentimg->save(current->nome.c_str());
      #ifdef DEBUG
         std::cout << "Salvata immagine: " << current->nome << std::endl;
      #endif
      delete currentimg;
      delete current;
   }
}




// funzione per applicare la firma ad una immagine. La funzione prende due parametri, una immagine firma e una immagine image su cui applicare la firma. Per accedere alle immagini si utilizza il campo data
// Solo quando un pixel della firma e' nero allora si ha la media tra il pixel nero della firma e il pixel dell'immagine image portato su una scala di grigio
void applicafirma(immagine* image, CImg<IMGT>* firma){
   CImg<IMGT>* img = image->img;
   unsigned char* ptr = img->data();
   unsigned char* ptrf = firma->data();
   int width = img->width();
   int height = img->height();
   int size = width*height;

   for (int t = 0; t < height; t++){
      for (int c = 0; c < width; c++){

         if(*(ptrf+t*width+c) == 0){
            int r = *(ptr+t*width+c);
            int g = *(ptr+size+t*width+c);
            int b = *(ptr+size*2+t*width+c);
            int media = (r+g+b)/3;
            media = media/2;
            //cout << "Thr:  ("<< t <<","<< c << ") = "<< r << " " << g << " " << b << " media: " << media << endl;
            *(ptr+t*width+c)=media;
            *(ptr+size+t*width+c)=media;
            *(ptr+size*2+t*width+c)=media;
         }
      }
   }
}
