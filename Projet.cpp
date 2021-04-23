#include <iostream>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define SCREEN_WIDTH 52
#define SCREEN_HEIGHT 20

#define MINX 2
#define MINY 2
#define MAXX 49
#define MAXY 19

using namespace std;
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int Briques[24][2] = {		// Position des briques
						{2,7},{2,12},{2,17},{2,22},{2,27},{2,32},{2,37},{2,42},
					  	{4,7},{4,12},{4,17},{4,22},{4,27},{4,32},{4,37},{4,42},
					  	{6,7},{6,12},{6,17},{6,22},{6,27},{6,32},{6,37},{6,42}
					};

int BriquesVis[24] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int barrePos[2] = {18,22}; // Position de la barre 
int BallePos[2] = {17,26}; // Position de la balle
int JouerB = 0;
int dir = 1; 
int BriqueG = 24;
int Victoire = 0;		// Gagner
int Perdu = 0; 			// Perdre

void Tr(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void Curseur(bool visible, DWORD Taille) 
{
	if(Taille == 0)
	{
		Taille = 20;	
	}
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = Taille;
	SetConsoleCursorInfo(console,&lpCursor);
}

void Champs(){
	Tr(0,0);cout<<"----------------------------------------------------";
	Tr(0,SCREEN_HEIGHT);cout<<"----------------------------------------------------";
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		Tr(0,i); cout<<"|";
		Tr(SCREEN_WIDTH,i); cout<<"|";
	}
}

void Briques1(){  				// Affichage des briques
	for( int i=0; i<24; i++){
		if( BriquesVis[i] == 1 ){ 
			Tr(Briques[i][1], Briques[i][0]);
			cout<<"|_XX_|"; 
		}
	}
}

void BarreBalle(){
	if( BallePos[1]>=barrePos[1] && BallePos[1]<=barrePos[1]+8){
		if( BallePos[0] == barrePos[0]-1 ){
			if( dir == 3 ) 
				dir = 2;
			else if( dir == 4 )
				dir = 1;
		}
	}
}

void BarreBrique(){
	for( int i=0; i<24; i++){
		if(BriquesVis[i]==1){
			if( BallePos[1]>=Briques[i][1] && BallePos[1]<=Briques[i][1]+8){
				if( BallePos[0] == Briques[i][0] ){
					BriquesVis[i] = 0;
					BriqueG--;
				}
			}
		}
	}
}

void Start(){                  // Fonction de démarrage du programme
	while(1){
		system("cls");
		Briques1();
		Champs();

		Tr(barrePos[1],barrePos[0]);
		cout<<"#############";

		Tr(BallePos[1],BallePos[0]);
		cout<<"C++"; 			// Affichage de la balle 
 
		if(kbhit()){
			char Ts = getch();
			if( Ts=='d'||Ts=='D'|| Ts==77 ){
				if(barrePos[1] < 44)
					barrePos[1] = barrePos[1]+2;
			} 
			if( Ts=='q'||Ts=='Q'|| Ts==75 ){
				if(barrePos[1] > 2)
					barrePos[1] = barrePos[1]-2;
			} 
			if(Ts==32){
				JouerB = 1;
			} 
			if(Ts==27){
				break;
			}
		}
		
		if( JouerB == 1 ){
			if( dir == 1) { 
				BallePos[0] = BallePos[0] - 1;
				BallePos[1] = BallePos[1] + 2;
				if( BallePos[1] > MAXX ){
					dir = 2;
				}  
				else if( BallePos[0] < MINY ){
					dir = 4;
				}   
			}
			else if( dir == 2) { 
				BallePos[0] = BallePos[0] - 1;
				BallePos[1] = BallePos[1] - 2;
				if( BallePos[0] < MINY ){
					dir = 3;
				}  
				else if( BallePos[1] < MINX  ){
					dir = 1;
				}   
			}
			else if( dir == 3) { 
				BallePos[0] = BallePos[0] + 1;
				BallePos[1] = BallePos[1] - 2;
			  
				if( BallePos[0] > MAXY ){
					Perdu = 1;
					break;
				}  
				else if( BallePos[1] < MINX  ){
					dir = 4;
				}    
			}
			else if( dir == 4) { 
				BallePos[0] = BallePos[0] + 1;
				BallePos[1] = BallePos[1] + 2;  
				if( BallePos[1] > MAXX ){
					dir = 3;
				} 
				else if( BallePos[0] > MAXY ){
					Perdu = 1;
					break;
				}
			}

			BarreBalle();
		}
		
		BarreBrique();
	
		if( BriqueG == 0){
			Victoire = 1;	
			break;
		}		

		Sleep(30);
	}
	
	if( Perdu == 1){    // Affichage de 'Perdu' lors de la défaite (lorsque la balle est tombée et qu'il reste des briques)
		system("cls");
		
		Tr(10,5); cout<<" --------------------- "; 
		Tr(10,6); cout<<" |     Perdu      	  | "; 
		Tr(10,7); cout<<" --------------------- "; 			

		Tr(10,9); cout<<"Pressez la touche 'Entree' pour revenir au menu."; 	
		getch(); 
	}

	if( Victoire == 1){  // Affichage de 'Victoire' lorsqu'on a fini de casser toutes les briques
		system("cls");
		Tr(10,5); cout<<" --------------------- "; 
		Tr(10,6); cout<<" |     Victoire      | "; 
		Tr(10,7); cout<<" --------------------- "; 			

		Tr(10,9); cout<<"Pressez la touche 'Entree' pour revenir au menu.";
		getch(); 	  			 
	}
}

void instructionsJeu(){  // Fonction d'affichage des instruction du programme
	
	system("cls");
	cout<<"Instructions du jeu ";
	cout<<"\n----------------";
	cout<<"\n1. Pressez la touche 'Q' pour vous diriger vers la gauche";
	cout<<"\n2. Pressez la touche 'D' pour vous diriger vers la droite";
	cout<<"\n3. Appuyer sur 'Espace' pour que la partie commence";
	cout<<"\n\nPressez la touche 'Entree' pour revenir au menu.";
	getch();
}

int main()    // Fonction principale du programme
{
	Curseur(0,0);  
	
	do{
		system("cls");
		Tr(10,5); cout<<" -------------------------- "; 
		Tr(10,6); cout<<" |     JEU DE BRIQUES     | "; 
		Tr(10,7); cout<<" --------------------------";
		Tr(10,9); cout<<"1. Commencer la partie";
		Tr(10,10); cout<<"2. Instructions du jeu";	 
		Tr(10,11); cout<<"3. Quitter le jeu";
		Tr(10,13); cout<<"Faites votre choix: ";
		char op = getche();
		
		if( op=='1') Start();
		else if( op=='2') instructionsJeu();
		else if( op=='3') exit(0);
		
	}while(1);

	Start();

	cout<<endl<<endl;	
	return 0;
}