

// RING CLIP
module ring_clip() {

// Main arc parameters
outer_radius = 86;
inner_radius = 78;
arc_angle = 30;
arc_height = 2;

// Small arc segment parameters
block_width = 4;         // radial width
block_angle = 10;        // degrees covered by each block
block_height = 3.5;      // block extrusion height
block_offset = 4;        // outward shift for small arcs
epsilon = 0.01;          // small overlap to ensure manifold


    
// Helper: create a ring arc (no center point, no straight slice)
module ring_arc(r1, r2, a1, a2, fragments=100) {
    step = (a2 - a1) / fragments;
    points = concat(
        [for(a = [a1 : step : a2]) [r1 * cos(a), r1 * sin(a)]],
        [for(a = [a2 : -step : a1]) [r2 * cos(a), r2 * sin(a)]]
    );
    polygon(points);
}


// Main arc (not shifted)
difference() {
    linear_extrude(height=arc_height)
        ring_arc(outer_radius, inner_radius, -arc_angle/2, arc_angle/2);

    linear_extrude(height=arc_height)
        ring_arc(outer_radius, inner_radius- epsilon, -arc_angle/6, arc_angle/6);
}

// Placement angles
left_angle = -arc_angle/2;
right_angle = arc_angle/2;

// LEFT blocks: left edge aligns with main arc end, block extends right
// Left Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        left_angle,
        left_angle + block_angle
    );

// Left bottom extends over to touch right block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        left_angle,
        left_angle + block_angle + 10
    );



// RIGHT blocks: right edge aligns with main arc end 
// Right Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        right_angle - block_angle,
        right_angle
    );

// Right bottom block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        right_angle - block_angle,
        right_angle
    );
    
  // tab
  translate([outer_radius, -7, 0])
    cube([20, 14, 3]);
}

// RING CLIP Mold
module ring_clip_mold() {

// Main arc parameters
outer_radius = 86;
inner_radius = 78;
arc_angle = 29;
arc_height = 1.65;

// Small arc segment parameters
block_width = 4;         // radial width
block_angle = 10;        // degrees covered by each block
block_height = 3.5;      // block extrusion height
block_offset = 4;        // outward shift for small arcs
epsilon = 0.01;          // small overlap to ensure manifold


    
// Helper: create a ring arc (no center point, no straight slice)
module ring_arc(r1, r2, a1, a2, fragments=100) {
    step = (a2 - a1) / fragments;
    points = concat(
        [for(a = [a1 : step : a2]) [r1 * cos(a), r1 * sin(a)]],
        [for(a = [a2 : -step : a1]) [r2 * cos(a), r2 * sin(a)]]
    );
    polygon(points);
}


// Main arc (not shifted)
difference() {
    linear_extrude(height=arc_height)
        ring_arc(outer_radius, inner_radius, -arc_angle/2, arc_angle/2);

   linear_extrude(height=arc_height)
        ring_arc(outer_radius, inner_radius- epsilon, -arc_angle/6, arc_angle/6);
}

// Placement angles
left_angle = -arc_angle/2;
right_angle = arc_angle/2;

// LEFT blocks: left edge aligns with main arc end, block extends right
// Left Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        left_angle,
        left_angle + block_angle +10
    );

// Left bottom extends over to touch right block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        left_angle,
        left_angle + block_angle + 10
    );



// RIGHT blocks: right edge aligns with main arc end 
// Right Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        right_angle - block_angle,
        right_angle
    );

// Right bottom block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        right_angle - block_angle,
        right_angle
    );
    
}

// RING CLIP Mold
module ring_clip_mold_minus() {

// Main arc parameters
outer_radius = 86;
inner_radius = 78;
arc_angle = 29;
arc_height = 1.65;

// Small arc segment parameters
block_width = 4;         // radial width
block_angle = 10;        // degrees covered by each block
block_height = 3.5;      // block extrusion height
block_offset = 4;        // outward shift for small arcs
epsilon = 0.01;          // small overlap to ensure manifold

    linear_extrude(height=arc_height)
        ring_arc(outer_radius, inner_radius, -arc_angle/2, arc_angle/2);


    
// Helper: create a ring arc (no center point, no straight slice)
module ring_arc(r1, r2, a1, a2, fragments=100) {
    step = (a2 - a1) / fragments;
    points = concat(
        [for(a = [a1 : step : a2]) [r1 * cos(a), r1 * sin(a)]],
        [for(a = [a2 : -step : a1]) [r2 * cos(a), r2 * sin(a)]]
    );
    polygon(points);
}

// Placement angles
left_angle = -arc_angle/2;
right_angle = arc_angle/2;

// LEFT blocks: left edge aligns with main arc end, block extends right
// Left Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        left_angle,
        left_angle + block_angle +10
    );

// Left bottom extends over to touch right block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        left_angle,
        left_angle + block_angle + 10
    );



// RIGHT blocks: right edge aligns with main arc end 
// Right Top block
linear_extrude(height=block_height)
    ring_arc(
        inner_radius - block_width + block_offset + epsilon,
        inner_radius - 2*block_width + block_offset - epsilon + 2, // Thin out
        right_angle - block_angle,
        right_angle
    );

// Right bottom block
linear_extrude(height=block_height)
    ring_arc(
        outer_radius + block_offset + epsilon - 2,
        outer_radius - block_width + block_offset - epsilon,
        right_angle - block_angle,
        right_angle
    );
    
}

//==

// RING CLIP Mold Minus Center
module ring_clip_mold_minus_center() {

  // Main arc parameters
  outer_radius = 86;
  inner_radius = 77.9;
  arc_angle = 34;
  arc_height = 6;

  linear_extrude(height=arc_height)
   ring_arc(outer_radius, inner_radius, -arc_angle/2, arc_angle/2);

  // Helper: create a ring arc (no center point, no straight slice)
  module ring_arc(r1, r2, a1, a2, fragments=100) {
    step = (a2 - a1) / fragments;
    points = concat(
        [for(a = [a1 : step : a2]) [r1 * cos(a), r1 * sin(a)]],
        [for(a = [a2 : -step : a1]) [r2 * cos(a), r2 * sin(a)]]
    );
    polygon(points);
  }
}


// GATE BOX
module gate_box() {
    
  gb_outside_x = 60;
  gb_outside_y = 100;
  gb_outside_z = 40;
  gb_wall_thick = 2;
  front_face_angle = 70;
  front_face_rise = 2.96;

  module gb_outside() {
    difference() {
        cube([gb_outside_x, gb_outside_y, gb_outside_z]);
        rotate(front_face_angle, [1,0,0] ) 
            translate ([0,0,front_face_rise]) 
                cube([gb_outside_x, gb_outside_y, gb_outside_z]);  
        rotate(18, [0,1,0] ) 
            translate ([-22.7,0,8.3]) 
                cube([20, gb_outside_y+1, gb_outside_z]);
        rotate(-18, [0,1,0]) 
          translate ([59.7,0,-10.3]) 
             cube([20, gb_outside_y+1, gb_outside_z]);
    }
  }

  module gb_inside() {
    difference() {
        cube([gb_outside_x-(gb_wall_thick*2), gb_outside_y-(gb_wall_thick*2), gb_outside_z-gb_wall_thick]);
        rotate(front_face_angle, [1,0,0] ) 
            translate ([0,0,front_face_rise]) 
                cube([gb_outside_x-(gb_wall_thick*2), gb_outside_y-(gb_wall_thick*2), gb_outside_z-gb_wall_thick]); 
        rotate(18, [0,1,0] ) 
            translate ([-22.7,0,8.3]) 
                cube([20, gb_outside_y-(gb_wall_thick*2)+4, gb_outside_z-gb_wall_thick]);
        rotate(-18, [0,1,0]) 
          translate ([56,0,-9]) 
             cube([20, gb_outside_y-(gb_wall_thick*2)+4, (gb_outside_z-gb_wall_thick)]);
    }
  }
  
  difference() { 
        gb_outside();
      
        // Remove the inside
        translate ([gb_wall_thick, gb_wall_thick, 0])
            gb_inside();
      
        // Make a curved slot by subtracting the ring clip
        translate([30, 38.17, 108]) rotate(-90, [0,0,1])  rotate(70, [0,1,0])  ring_clip_mold_minus_center();
  
        // Make the LCD display opening
        translate ([(gb_outside_x/2) , 3.85, 19.4]) 
           rotate(front_face_angle+180, [1,0,0]) 
              linear_extrude(height=12, scale=0.1)  square([28, 8], center=true); // inward taper on all sides
      
        // Make the USB Power opening
        translate ([(gb_outside_x/2)-(10.5/2) , gb_outside_y-4, 23.5])  
          cube([10.5, 8, 7]);
  }
 
  // Add LCD display top holder
  translate ([(gb_outside_x/2)-(26.3/2) , 11, 21.7]) 
      rotate(front_face_angle, [1,0,0]) 
        cube([26.3, 2, 4]);
        
  // Add LCD display bottom holder
  translate ([(gb_outside_x/2)-(26.3/2) , 6, 8.96]) 
      rotate(front_face_angle, [1,0,0]) 
        cube([26.3, 2, 4]);
        
}

// GATE Lid
module gate_lid() {
   
  gbl_wall_thick = 2; 
  gbl_outside_x = 60;
  gbl_outside_y = 100;
  gbl_outside_z = gbl_wall_thick;

  cube([gbl_outside_x, gbl_outside_y, gbl_outside_z]);
  
  translate([gbl_wall_thick, gbl_wall_thick, gbl_wall_thick])
    difference() { 
      cube([gbl_outside_x-(gbl_wall_thick*2), gbl_outside_y-(gbl_wall_thick*2), gbl_outside_z]);
        translate([gbl_wall_thick, gbl_wall_thick, 0])
          cube([gbl_outside_x-(gbl_wall_thick*4), gbl_outside_y-(gbl_wall_thick*4), gbl_outside_z]);
    }
 }

// Main - Generate Objects{ 
gate_box();

//translate ([-70,0,0]) gate_lid();


// Now add the clip back
// translate([30, 38.17, 108]) rotate(-90, [0,0,1])  rotate(70, [0,1,0])  ring_clip_mold();

//translate([20, 0, 0]) ring_clip();


        

  
    
