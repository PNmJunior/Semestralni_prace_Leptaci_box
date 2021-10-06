/*
Tento soubor je popisem komponent pomoci jazyka použivaného programem OpenSCAD. https://openscad.org/
*/

//3D tisk rozvržení komponentů:


//Komponenty:

//KulUchytSt();
//HranUchytSt();
//Nosnik();
//Spojka();


//met= zkratka pro mertix system
metVysNosnik= 30;//globalni proměnná, která určuje výšku nosníku.
//globalní proměnná, která určuje výšku nástavců přidržujících plošný spoj.
metVysDrz = metVysNosnik+55+0-5; //vyskaNosniku + výška plošiny v dolní poloze + rezerva - rozdíl mezi x a y tyčkou 


//KulUchytSt();
module KulUchytSt()//Kulatý nástavec na držení spoje.
{
KulUchyt(metVysDrz, 30,1.5,2,9,9);
}
module KulUchyt(vyska,vyskaValce , hloubkaVyrez, pocetHranVyrezy,sirkaValce, sirkaNast = 9)
{
    translate([0,0,vyskaValce]) Nastavec(vyska-vyskaValce,7);
    translate([(sirkaNast)/2,(sirkaNast)/2,0]) difference()
    {
        cylinder(vyskaValce + 5,d = sirkaValce, center = false);
        translate([0,0,hloubkaVyrez+ 0.2])
        {
        rotate_extrude(convexity = 10) translate([sirkaValce/2, 0,0])
            //polygon( points=[[0,0],[1,1],[2,0], [0,0] );
            circle(r = hloubkaVyrez, $fn = pocetHranVyrezy *2);
        }
    }
}

//HranUchytSt();
//HranUchyt(metVysDrz);
module HranUchytSt()//Hranatý nástavec na držení spoje.
{
    HranUchyt(metVysDrz);
}


module HranUchyt(vyska, sirka = 9)
{
    difference()
    {
        Nastavec(vyska,7,sirka);
        translate([0,0,1.5])
        {
        translate([0,0,0]) rotate([0,45,0]) cube([2,30,2],center = true);
        translate([0,0,0]) rotate([0,45,90]) cube([2,30,2],center = true);
        translate([0,sirka,0]) rotate([0,45,90]) cube([2,30,2],center = true);
        translate([sirka,0,0]) rotate([0,45,00]) cube([2,30,2],center = true);
        }
    }
}

//Nosnik(3);
//Nosnik();
module Nosnik(vyskaRov = 4)// Převod mezi pohyblivou částí CD rom a tyčkami, pomoci kterých jde nastavit šířka plošného spoje. Je nutné, aby byl Nosník pevný a odolný, a proto to tisknu vicekrát ze dvou pozic a dvou rozdměrů desky.
{
    deska = 9;
    odDesky = 5;
    velNast = 9;
    sirka = 27;
    translate([(sirka + velNast)/2,0,0])rotate([0,0,90]) Nastavec(30,4,velNast);
    difference()
    {
         OvalnaKrichleCisla([sirka,velNast+odDesky+deska,vyskaRov],1,1,1);
         translate([0,velNast+odDesky,0]) DiryNaDrzak();
    }
    difference()
    {
        translate([0,0,vyskaRov-4 ])
        {
    translate([13,9,0]) rotate([0,-30,130]) cube([8,4,15]);
    translate([17,6,0]) rotate([0,-30,45]) cube([8,4,15]);
        }
        translate([0,0,-10])cube([30,30,10]);
    }
}


NosnikV2();
module NosnikV2(vyskaRov = 4, delkaRov = 100)// Převod mezi pohyblivou částí CD rom a tyčkami, pomoci kterých jde nastavit šířka plošného spoje. Je nutné, aby byl Nosník pevný a odolný, a proto to tisknu vicekrát ze dvou pozic a dvou rozdměrů desky.
{
    deska = 9;
    odDesky = 5;
    velNast = 8;
    sirka = 27;
    delkaNavic = 1;
    vyskaNast = 24;
    diryRozdm = 3.1;
    diryStart = (velNast+delkaNavic+(3/2)+3);
    diryKrok = 3*2;//diryRozdm*2;
    diryStop = (delkaRov - (3/2)-10);
    
    translate([0,0,0]) difference()
    {
    translate([0,0,0])
        {
    translate([(sirka-velNast)/2,0,vyskaRov]) rotate([-20,0,0]) OvalnaKrichleCisla([velNast,10,vyskaNast-2.8],1,0.5,0.5);
            
    translate([(sirka-velNast)/2,7,0]) OvalnaKrichleCisla([velNast,delkaRov-7,vyskaNast],1,0.5,0.5);
            
            
        }
        for(i=[diryStart:diryKrok:diryStop])
        {
            translate([0,i,3]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,13]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,8]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,18]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            
            translate([0,3,2.5]){
            translate([0,i,3]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,13]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,8]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([0,i,18]) rotate([0,90,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            }
            
            translate([0,0,2.5]){
            translate([(sirka)/2,delkaRov-17,3]) rotate([-90,0,0]) cylinder(20, d = diryRozdm, true,$fn=20);
                //translate([(sirka)/2,delkaRov-20,8]) rotate([-90,0,0]) cylinder(20, d = diryRozdm, true,$fn=20);
                translate([(sirka)/2,delkaRov-17,13]) rotate([-90,0,0]) cylinder(20, d = diryRozdm, true,$fn=20);
            translate([(sirka-velNast)/2,delkaRov-2-2,0-1]) cube([velNast,2,8]);
            translate([(sirka-velNast)/2,delkaRov-2-2,10-1]) cube([velNast,2,8]);
            }
            ///translate([(sirka-velNast)/2,delkaRov-2-2,2]) cube([velNast,2,7]);
            //translate([(sirka-velNast)/2,delkaRov-2-2,]) cube([velNast,2,7]);
            
        }
        
        for(i=[diryStart:diryKrok*2:diryStop])
        {
            translate([sirka/2,i,0]) cylinder(sirka, d = diryRozdm, true,$fn=20);
            translate([sirka/2,i,vyskaNast-1]) cylinder(sirka, d = 7, true,$fn=20);
        }
        
        
        
        
        
    }
        translate([sirka,+delkaNavic + velNast,0]) rotate([0,0,180])difference()
    {
         OvalnaKrichleCisla([sirka,velNast+delkaNavic,vyskaRov],0.5,0.5,0.5);
         translate([0,delkaNavic,0]) DiryNaDrzak();
    }
    }


module DiryNaDrzak()//přesně naměřeno
{
    sirkaObruby = 3.75;
    vyskaObruby = 4;
	
    translate([2,6,0]){
        cylinder(vyskaObruby,d=sirkaObruby,center=true,$fn=20);
        cylinder(10,d=2,center=true,$fn=20);
        
    }
    //translate([10.75+1.5,4,0]) cylinder(10,d=3,center=true,$fn=20);
    translate([24.5,2.5,0]){
        cylinder(vyskaObruby,d=sirkaObruby,center=true,$fn=20);
        cylinder(10,d=2,center=true,$fn=20);
        translate([0,-sirkaObruby/2,-vyskaObruby/2])cube([100,sirkaObruby,vyskaObruby],center = false);
    }
    
    translate([25.2,-40,0]) rotate([0,20,0]) cube([100,100,100]);
}


//Spojka();
module Spojka()//Jde o převodník mezi "Nosníkem" a "Nástavci".
{
    Nastavec(5*5+5,5);
}


//Univerzálni nástavec
module Nastavec(vyska,pocetDer,sirka = 9)
{
    //OvalnaKrichleCisla([10,10,vyska],1,1,1);
    difference()
    {
        
        OvalnaKrichleCisla([sirka ,sirka ,vyska],1,1,1);
        translate([0,0,2.5])
        {
            for(i = [0 : 1: pocetDer-1])
            {

            translate([sirka/2,sirka/2,vyska - 5-i*5]) rotate([0,90,0]) cylinder(h=sirka*2,d = 3.1,center = true,$fn=20);
            translate([sirka/2,sirka/2,vyska - 7.5-i*5]) rotate([90,0,0]) cylinder(h=sirka*2,d = 3.1,center = true,$fn=20);
                
            }
        }
    }
}


//Kód pro vytvoření oválných kvádrů
module OvalnaKrichleX(Rozdmery, zakulaceni)
{
	union() {
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1] - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([0,zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,Rozdmery[1] - zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,Rozdmery[1] - zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	}
}


module OvalnaKrichleY(Rozdmery, zakulaceni)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	}
}


module OvalnaKrichleZ(Rozdmery, zakulaceni)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1]  - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	}
}


module OvalnaKrichle(Rozdmery, zakulaceni, osaX, osaY, osaZ)
{
	cont = false;
	intersection()
	{
		if(osaX == true)
		{
			OvalnaKrichleX(Rozdmery,zakulaceni);
			cont = true;
		}
		if(osaY == true)
		{
			OvalnaKrichleY(Rozdmery,zakulaceni);
			cont = true;
		}
		if(osaZ == true)
		{
			OvalnaKrichleZ(Rozdmery,zakulaceni);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}


module OvalnaKrichleCisla(Rozdmery, osaX, osaY, osaZ)
{
	cont = false;
	intersection()
	{
		if(osaX != 0)
		{
			OvalnaKrichleX(Rozdmery,osaX);
			cont = true;
		}
		if(osaY != 0)
		{
			OvalnaKrichleY(Rozdmery,osaY);
			cont = true;
		}
		if(osaZ != 0)
		{
			OvalnaKrichleZ(Rozdmery,osaZ);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}