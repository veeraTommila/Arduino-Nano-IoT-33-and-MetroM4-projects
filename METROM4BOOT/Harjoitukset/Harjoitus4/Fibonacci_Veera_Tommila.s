.global _start
_start:
	//Veera Tommila, veera.tommila@gmail.com.
	//Harjoitus 4. Fibonacciluvut 0,1,2,3,5,8,13,21,34.
	MOV R0,#1	//Ensimmäinen fibonacciluku.
	MOV R1,#2	//Toinen fibonacciluku.
	MOV R2,#0	//Kierrosaskuri.
	MOV R3,#0	//Summa.	
	MOV R4,#0	//Summa.	
	/*
	Eli Fibonacci-luvut alkavat näin: 0,1,1,2,3,5,8,13,21,34,55,…  ja kahdeksan ensimmäistä ovat siis 0,1,1,2,3,5,8,13  ja summaksihan tulee 33. 
	Kuten Arduinon koodissakin ensimmäinen ja toinen Fibonacci-luku tiedetään (0 ja 1) ja niiden summa tiedetään (1). 
	Nämä voidaan sijoittaa aluksi rekistereihin r0, r1 ja r3 (tietysti rekisterit saa valita vapaasti mitä käyttää). 
	Sitten loopissa kierretään kuusi kierrosta ja joka kierroksella lasketaan kahden edellisen fiboluvun arvot yhteen (tämähän vaatii oman rekisterin mihin se lasketaan, esim r4), josta saadaan seuraavan fiboluvun arvo ja tämä arvo lisätään summaan. 
	Lisäksi joka kierroksella pitää päivittää rekistereiden arvot uutta kierrosta varten eli rekisteriin r0 siirretään rekisterin r1 arvo ja rekisteriin r1 siirretään rekisterin r4 arvo (eli se uusin luku joka just laskettiin). 
	Myös laskurin arvoa kasvatetaan yhdellä ja lopuksi verrataan onko se yhtä suuri kuin 6 (BNE).
	*/
LOOP:
	ADD R4,R0,R1	//R4 = R0 + R1. Lasketaan ensimmäinen ja toinen luku yhteen muuttujaan R4.
	//ADD R3,R4	//R4 = R4 + R3. Lisätään kahden ensimmäisen luvun yhteenlaskettu arvo R4 summaan R3.
	MOV R0,R1	//Sijoitetaan R1:sen arvo muuttujaan R0. 
	MOV R1,R4	//Sijoitetaan R4:sen arvo muuttujaan R1. 
    ADD R2,#1	
    CMP R2,#6   
 	BNE LOOP

_end:
	