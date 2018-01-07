$fn=100;

// Breite der Schlaufe
breite= 36;
    

module retroUhr()
{
  color("LightCoral") cylinder(d=42.8, h= 1.5);
  
  color("darkred") translate([-15.37/2, 8, 1.5]) 
  {
    cube([15.37, 6.35, 3]);
    
    translate([0.5*3.84, 6.35/2, 3]) resize([3.84,6.35,2.54]) sphere(2.54);
    translate([1.5*3.84, 6.35/2, 3]) resize([3.84,6.35,2.54]) sphere(2.54);
    translate([2.5*3.84, 6.35/2, 3]) resize([3.84,6.35,2.54]) sphere(2.54);
    translate([3.5*3.84, 6.35/2, 3]) resize([3.84,6.35,2.54]) sphere(2.54);
  }
  
  color("silver") translate([0,-4,4.5]) cylinder(d=20,h=2);
  
  color("darkgrey")
  {
    translate([ 41.7/2-3.5, -3, 1.5]) cube([3.5, 6, 3.5]);
    translate([-41.7/2    , -3, 1.5]) cube([3.5, 6, 3.5]);
  }
}

module armband()
{
  laenge= 56;
  color("DarkSlateBlue")
    translate([-26/2,-laenge/2, -3])
      cube([26, laenge, 3]);
      
  *color("darkgrey")
    translate([-30.5/2, -laenge/2, -6])
      cube([30.5, laenge, 3]);
  
}

module schlaufe()
{
  difference()
  {
    union()
    {
      translate([-breite/2, 47/2-12, -2]) cube([breite, 12, 7]);
      translate([-breite/2, 47/2, 7-2]) rotate([-25,0,0]) difference()
      {
        union()
        {
          translate([0,0,-10]) cube([breite, 9, 10]);
          translate([0,-3,-12]) cube([breite, 9, 10]);
        }
        translate([(breite-26)/2, 6.19, -5.63])cube([26,20,3]);
      }
    } 
    translate([0,0,0]) cylinder(d= 46, h=15);
    
    translate([0,0,-5]) cylinder(d= 43.4, h=15);
  
  translate([-25, 47/2-8, -22]) rotate([0,90,0]) cylinder(d=40,h=50);  
    *translate([-26/2,47/2-4,-6]) rotate([30,0,0]) cube([26, 15,3]);
  }
  
  
  *translate([-breite/2, 47/2-4, 7-2]) rotate([-25,0,0]) translate([0,0,-7])cube([breite, 12, 7]);
}
module gehauese()
{
  difference()
  {
    union()
    {
      translate([0,0,-2]) cylinder(d= 47, h=7);
      schlaufe();
      rotate([0,0,180]) schlaufe();
    }
    
    translate([0,0,-2.5]) cylinder(d=43.4, h=5.5);
    translate([0,0,2]) cylinder(d=43.4-3.5, h=15);
     
    rotate([0,0,0]) translate([17.5,-3.5,-2]) cube([4,7,10]);
    rotate([0,0,180]) translate([17.5,-3.5,-2]) cube([4,7,10]);
    rotate([0,0,-65]) translate([17.5,-5,-2]) cube([4,10,10]);
    
    translate([-26/2,-28,-3.])cube([26, 2*28, 3]);
    
    *translate([0,0,-10]) cube([50,50,50]);
  }

  *difference()
  {
    union()
    {
      schlaufe();
      rotate([0,0,180]) schlaufe();
    }
    
    translate([-26/2,-47/2-1,-3.])cube([26, 49, 3]);
  }
}

module scheibe()
{
  %translate([0,0,9]) cylinder(d=47, h=3);
}

*#import("RetroUhr.bak.stl");


*schlaufe();
*gehauese();
*scheibe();
*armband();

*translate([0,0,.75]) retroUhr();

rotate([180,0,0])gehauese();