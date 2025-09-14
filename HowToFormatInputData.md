These instructions define the required format for Wall and Window dimension data in a .txt file to ensure correct parsing and processing by the program:



**File Structure and Syntax Specification**

The first line of the file must be a header.

Subsequent lines contain object data in the specified format.

Column values are space-separated.

The Name column uniquely identifies each wall or window for clarity and ease of modification.

Brackets () for coordinates and double-quotes "" for strings must be included.



**Coordinate System**

All objects are projected onto an xy-plane.

The origin (0,0) is set at the left-bottom corner of the wall.

Coordinates are specified in (x,y) form.



**Measurements**

Thickness is entered as a single numeric value.

All measurements are in metres.



**Object Specification**

Walls: Specify coordinates for the left-top and right-bottom corners, along with thickness.

Windows: Modeled in the same plane as the wall they belong to; thickness equals that of the associated wall.

Roof and Floor: If made of bricks, treat as walls and include with type "Wall".

Bricks: Keeping the brick with the longest side (length) in front, take the vertical distance as the
height and the distance from the front-face to the back-face as the thickness

**Line Format**

Object-Type Object-Name Left-Top Right-Bottom Thickness

**Examples:**
//What if I only write specifically what I need (since the instruction document is also there)
eg. Wall Kitchen-cabinet-wall 10 10 2 Concrete
"Wall" "Kitchen-cabinet-wall" (0,10) (10,0) 2

"Window" "Front-french-windows" (0,5) (5,0) 2

