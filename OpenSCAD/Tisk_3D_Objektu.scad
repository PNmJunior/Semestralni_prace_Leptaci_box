/*
Tento soubor je popisem komponent pomoci jazyka použivaného programem OpenSCAD. https://openscad.org/
*/

//TiskNutneMinimum();
module TiskNutneMinimum()//Rozložení komponent pro tisk na malé ploše 100x120mm. Jsou zde nejnutnější komponenty.
{
    translate([0,0,0])SenSroub();
    translate([0,57,0])SenHoriz();
    translate([0,90,0])Nosnik();
    translate([25,60,0]) rotate([90,0,90])Nosnik();
    
    translate([90,80,0]) rotate([90,-90,0]){
    translate([0,0,0]) rotate([0,0,270]) HranUchytSt();
    translate([0,10,0]) rotate([0,0,270])HranUchytSt();
    translate([0,20,0]) rotate([0,0,270])HranUchytSt();
    translate([0,30,0]) rotate([0,0,270])HranUchytSt();
    }
    translate([100,95,0]) rotate([0,-90,0]){
    translate([0,0,0]) rotate([0,0,270]) Spojka();
    translate([0,10,0]) rotate([0,0,270])Spojka();
    translate([0,0,35]) rotate([0,0,270])Spojka();
    translate([0,10,35]) rotate([0,0,270])Spojka();
    }
}


//TiskVsechnyKomponenty();
module TiskVsechnyKomponenty()//Rozložení komponent pro tisk na větší ploše 150x130mm. Jsou zde komponenty pro tvorbu variací konfigurace držáku plošného spoje.
{
    translate([0,0,0])SenSroub();
    translate([0,60,0])SenHoriz();
    translate([0,100,0])Nosnik();
    translate([30,100,0])Nosnik(3);
    translate([25,60,0]) rotate([90,0,90])Nosnik();
    
    translate([140,10,0]) rotate([0,-90,0]){
    translate([0,0,0])rotate([0,0,270]) KulUchytSt();
    translate([0,10,0])rotate([0,0,270]) KulUchytSt();
    translate([0,20,0])rotate([0,0,270]) KulUchytSt();
    translate([0,30,0])rotate([0,0,270]) KulUchytSt();
    }
    translate([140,50,0]) rotate([0,-90,0]){
    translate([0,0,0]) rotate([0,0,270]) HranUchytSt();
    translate([0,10,0]) rotate([0,0,270])HranUchytSt();
    translate([0,20,0]) rotate([0,0,270])HranUchytSt();
    translate([0,30,0]) rotate([0,0,270])HranUchytSt();
    }
    translate([140,90,0]) rotate([0,-90,0]){
    translate([0,0,0]) rotate([0,0,270]) Spojka();
    translate([0,10,0]) rotate([0,0,270])Spojka();
    translate([0,20,0]) rotate([0,0,270])Spojka();
    translate([0,30,0]) rotate([0,0,270])Spojka();
    }
    translate([100,82,0]) rotate([0,-90,0]){
    translate([0,0,0])  Spojka();
    translate([0,10,0]) Spojka();
    translate([0,20,0]) Spojka();
    translate([0,30,0]) Spojka();
    }
}

SenSroub();
module SenSroub()//Jde o krabičku, kde bude umístěn malý plošný spoj, na kterém bude infračervený senzor teploty. Krabička bude umístěná nad podložkou pomoci stavebnice Merkur.
{
    xO = 40;
    yO  =20;
    zO  = 8;
    zOp  = 2;
    hrana = 2;
    difference()
    {
        translate([0,0,0])
        {
        SenSroubJazik(xO,yO,zO,zOp,hrana);
            translate([10,0,0])OvalnaKrichleCisla([xO-10 ,yO ,zO],1,1,1);
        }
        translate([0,2,zOp])OvalnaKrichleCisla([xO-hrana ,yO - hrana*2 ,zO-zOp],1,1,1);
        translate([25,yO/2,0]) cylinder(10, d = 11,center  =true);
        translate([4,yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,yO/2,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,3*yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        
    }
    
    translate([0,yO,0]) SenSroubJazik(xO,yO,zO,zOp,hrana);
    translate([0,2* yO-hrana,0]) SenSroubJazikB(xO,yO,zO,zOp,hrana);
}

module SenSroubJazik(xO,yO,zO,zOp,hrana)
{
rezerva = 0.5;
    difference()
    {
        translate([0,0,0])
        {
        translate([0,hrana +rezerva ,0])OvalnaKrichleCisla([xO-rezerva-hrana ,yO-((hrana+rezerva)*2) ,zOp],1,1,1);
        }
        translate([4,yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,yO/2,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,3*yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        
    }
}

module SenSroubJazikB(xO,yO,zO,zOp,hrana)
{
rezerva = 0.5;
    difference()
    {
        translate([0,0,0])
        {
        //translate([0,hrana +rezerva ,0])OvalnaKrichleCisla([xO-rezerva-hrana ,yO-((hrana+rezerva)*2) ,zOp],1,1,1);
            translate([0,hrana +rezerva ,0])OvalnaKrichleCisla([15 ,yO-((hrana+rezerva)*2) ,zOp],1,1,1);
            translate([0,yO/3 ,0])OvalnaKrichleCisla([xO-rezerva-hrana , ,yO/3 ,zOp],1,1,1);
        }
        translate([4,yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,yO/2,0]) cylinder(10, d = 3,center  =true,$fn =20);
        translate([4,3*yO/4,0]) cylinder(10, d = 3,center  =true,$fn =20);
        
    }
}


vyskaSen = 2+4+1;//globálni proměnná pro výšku infračerveného senzoru
//SenHoriz();
module SenHoriz()//Jde o krabičku, kde bude umístěn malý plošný spoj, na kterém bude infračervený senzor teploty. Krabička bude upevněná na tyčku vycházející z "Nosníku"(Popis níže).
{
    xO = 20;
    yO  =30;
    zO  = 22.5;
    difference()
   { 
       
    OvalnaKrichleCisla([xO ,yO ,zO],1,1,1);
    translate([xO/2,yO/2,0])Senzor();
    translate([-2,3,vyskaSen])OvalnaKrichleCisla([xO +4 ,yO - 6 ,zO],1,1,1);
    translate([xO/2,-1,vyskaSen  + 10,])rotate([90,0,0]) cylinder(yO  +200,d=3,$fn = 10, center = true);
       translate([3.5,5.5,0]) 
       {
           cylinder(30,d = 3, center = true,$fn =20);
        cylinder(5,d = 5, center = true,$fn =20);
       }
       translate([xO-3.5,5.5,0]) 
       {
           cylinder(30,d = 3, center = true,$fn =20);
        cylinder(5,d = 5, center = true,$fn =20);
       }
       translate([xO-3.5,yO-5.5,0]) 
       {
           cylinder(30,d = 3, center = true,$fn =20);
        cylinder(5,d = 5, center = true,$fn =20);
       }
       translate([3.5,yO-5.5,0]) 
       {
           cylinder(30,d = 3, center = true,$fn =20);
        cylinder(5,d = 5, center = true,$fn =20);
       }

   }
}


//Senzor();
module Senzor()
{
    translate([0,0,-5]) cylinder(20,d=6,$fn = 10, center = false);
    translate([0,0,2]) cylinder(20,d=9,$fn = 16, center = false);
    translate([0,0,2+4]) cylinder(20,d=11,$fn = 16, center = false);
}


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
    translate([0,0,vyskaValce]) Nastavec(vyska-vyskaValce,5);
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
        Nastavec(vyska,5,sirka);
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


module DiryNaDrzak()//přesně naměřeno
{
    sirkaObruby = 3.75;
    vyskaObruby = 4;
	
    translate([2,6,0]){
        cylinder(vyskaObruby,d=sirkaObruby,center=true,$fn=20);
        cylinder(10,d=2,center=true,$fn=20);
        
    }
    translate([10.75+1.5,4,0]) cylinder(10,d=3,center=true,$fn=20);
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
        translate([0,0,vyska])
        {
            for(variable = [-5 : -5: -pocetDer*5])
            {
                if(variable % 10 == 0)
                {
                    translate([sirka/2,sirka/2,variable]) rotate([0,90,0]) cylinder(h=sirka*2,d = 3,center = true,$fn=10);
                }
                else
                {
                    translate([sirka/2,sirka/2,variable]) rotate([90,0,0]) cylinder(h=sirka*2,d = 3,center = true,$fn=10);
                }
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

