// ----------------------- Variable Globales ------------------------ //
const int bird_pixels[12][17] =
{
   { 2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2 },
   { 2,2,2,2,1,1,1,1,1,1,1,0,1,2,2,2,2 },
   { 2,2,2,1,1,1,1,1,1,1,0,0,0,1,2,2,2 },
   { 2,1,1,1,1,1,1,1,1,1,0,0,1,0,1,2,2 },
   { 1,0,0,0,0,1,1,1,1,1,0,0,1,0,1,2,2 },
   { 1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,2,2 },
   { 1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2 },
   { 2,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1 },
   { 2,2,1,1,1,1,1,1,1,1,0,1,1,1,1,1,2 },
   { 2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,1,2 },
   { 2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2 },
   { 2,2,2,2,2,2,2,1,1,1,1,2,2,2,2,2,2 }
};

// 128 x 64
// 126 x 64 es el espacio disponible
// Dividire la pantalla en cuadros de 6 x 6
// Seran 20 x 