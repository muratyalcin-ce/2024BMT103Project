#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINE_LEN 12 // 11 haneli TC + newline karakteri

int main(void) {

	// giris.txt ve casus.txt dosyalarini gerekli izinlerle ac ve degiskenlere ata
	FILE *fgiris = fopen("giris.txt", "r");
	FILE *fcasus = fopen("casus.txt", "w");

	// Dosyalarin basarili sekilde acilip acilmadigina dair kontroller
	if (fgiris == NULL) {
		printf("giris.txt dosya acma isleminda hata olustu (fopen)");
		exit(1);
	}

	if (fcasus == NULL) {
		fclose(fgiris);
		printf("casus.txt dosya acma isleminda hata olustu (fopen)");
		exit(1);
	}


	// karsilastirilma yapilacak tc lerin tutulacagi degiskenler 
	char tc[LINE_LEN];
	char tc2[LINE_LEN];


	// dosya icinde sifirdan islem yapmadan once eski pozisyonun tutulacagi degisken
	long eskipos = 0;

	// fread fonksiyonu ile giris.txt dosyasindan 12 serli sekilde tc degiskenine okuma islemi
	while (fread(tc, 1, LINE_LEN, fgiris)) {

		// ftell fonksiyonu ile eski posizyon degiskene atandi
		eskipos = ftell(fgiris);
		int c = 0; // Ayni tc ile kac kez karsilasildi

		// Dosya nin basina git
		fseek(fgiris, 0, SEEK_SET);


		// Bu sefer en bastan tc2 degiskenine okuma islemi yapildi 
		while (fread(tc2, 1, LINE_LEN, fgiris)) {

			// Bu kisimda tc ile tc2 degiskenlerinin her karakteri birbiriyle kiyaslaniyor, farkli byte varsa flag degiskenine 0 ataniyor
			bool flag = 1;
			for (int i = 0; i < LINE_LEN-1; i++) {
				if (tc[i] != tc2[i]) {
					flag = 0;
					break;
				}
			}

			// eger flag 1 ise karsilasma sayisi 1 arttiriliyor 
			if (flag) {
				c++;
			}
		}

		// eger ayni tc birden fazlaa kez bulunduysa fcasus degiskeni ile casus.txt dosyasina yaziliyor
		// hata var ise dosyalar kapatilip program sonlandiriliyor
		// hata yok ise ana donguden cikiliyor, dosyalar kapatiliyor ve program basarili bir sekilde sonlandiriliyor
		if (c > 1) {
			if(fwrite(tc, 1, LINE_LEN, fcasus) < LINE_LEN) {
				printf("casus.txt dosyasina yazmada hata olustu (fwrite)");
				fclose(fgiris);
				fclose(fcasus);

				exit(1);
			}

			break;
		}

		// eski pozisyona geri don
		fseek(fgiris, eskipos, SEEK_SET);
	}

   
	fclose(fgiris);
	fclose(fcasus);
	return 0;
}
