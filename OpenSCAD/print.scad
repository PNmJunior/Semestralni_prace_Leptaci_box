//HranUchyt();

module HranUchyt(vyska = 50, sirka = 9)
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

Nosnik();
module Nosnik()
{
    deska = 9;
    odDesky = 5;
    velNast = 9;
    sirka = 27;
    translate([(sirka + velNast)/2,0,0])rotate([0,0,90]) Nastavec(45,6,velNast);
    difference()
    {
         OvalnaKrichleCisla([sirka,velNast+odDesky+deska,4],1,1,1);
         translate([0,velNast+odDesky,0]) DiryNaDrzak();
    }
    translate([12,9,0]) rotate([0,-30,130]) cube([8,4,15]);
    translate([18,6,0]) rotate([0,-30,45]) cube([8,4,15]);
}


module DiryNaDrzak()//přesne naměřeno
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


module Spojka()
{
    Nastavec(5*5+5,5);
}


//Univerzalni nastavec
module Nastavec(vyska,pocetDer,sirka = 9)
{
    //OvalnaKrichleCisla([10,10,vyska],1,1,1);
    difference()
    {
        
        OvalnaKrichleCisla([sirka ,sirka ,vyska],1,1,1);
        translate([0,0,vyska]){
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


//Systemove Moduly
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

