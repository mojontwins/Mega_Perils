#include <allegro.h>
#include <stdio.h>
#include <stdlib.h> 

#define BLACK 0
#define WHITE 255

typedef struct {
	char fname_map [128];
	char fname_til [128];
	unsigned char map_w;
	unsigned char map_h;
	unsigned char scr_w;
	unsigned char scr_h;
	unsigned char nenem;
} MAPHEADER;

typedef struct {
	char t;				// tipo, si = 0, no existe.
	char x, y, xx, yy;	// all-purpose. En los lineales, si x == xx es vertical y si y == yy es horizontal.
	char n;				// velocidad del enemigo
	char s1, s2;		// spare. Por si es necesario para algún tipo de enemigo.
} ENEMYCHUNK;

typedef struct {
	unsigned char x, y;
    unsigned char tipo;	
} OBJPLACEMENT;

char input [16][128];//={"test.map", "test.bmp", "5", "15", "15", "9", "3", "", "", "", "", "", "", "", "", ""};

MAPHEADER header;				// Ahí, tó global. Al carajo. Tó lo que he aprendido en años, al carajo ;-)
ENEMYCHUNK tiestos [100][25];	// Por dios, pero que cacho de bejtia soy. Pero es que *no_tengo_ganas*
OBJPLACEMENT objs[100];
unsigned int maxt;

char signo (char a) {
	if (a < 0)
		return -1;
	else if (a == 0)
		return 0;
	else
		return 1;
}

DIALOG entrada[] = {

//		proc			x		y		w		h		fg		bg		key		flags		d1		d2		dp			dp2		dp3

	{	d_clear_proc,	0,		0,		0,		0,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_box_proc,		0,		0,		640,	480,	WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	8,		8,		320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"Crear un proyecto nuevo",	NULL,	NULL	},
	{	d_text_proc,	8,		32,		320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"Nombre de archivo de mapa (MAP)", NULL, NULL	},
	{	d_box_proc,		8,		48,		320,	10,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_edit_proc,	9,		49,		318,	8,		BLACK,	WHITE,	0,		0,			128,	0,		input [0],	NULL,	NULL	},
	{	d_text_proc,	8,		80,		320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"Nombre de archivo de tiles (BMP)", NULL, NULL	},
	{	d_box_proc,		8,		96,		320,	10,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_edit_proc,	9,		97,		318,	8,		BLACK,	WHITE,	0,		0,			128,	0,		input [1],	NULL,	NULL	},
	{	d_text_proc,	8,		128, 	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"MAP_W",	NULL,	NULL	},
	{	d_edit_proc,	64,		128,	40,		8,		BLACK,	WHITE,	0,		0,			6,		0,		input [2],	NULL,	NULL	},
	{	d_text_proc,	8,		144,	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"MAP_H",	NULL,	NULL	},
	{	d_edit_proc,	64,		144,	40,		8,		BLACK,	WHITE,	0,		0,			6,		0,		input [3], 	NULL,	NULL	},
	{	d_text_proc,	160,	128, 	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"SCR_W",	NULL,	NULL	},
	{	d_edit_proc,	224,	128,	40,		8,		BLACK,	WHITE,	0,		0,			6,		0,		input [4],	NULL,	NULL	},
	{	d_text_proc,	160,	144,	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"SCR_H",	NULL,	NULL	},
	{	d_edit_proc,	224,	144,	40,		8,		BLACK,	WHITE,	0,		0,			6,		0,		input [5], 	NULL,	NULL	},
	{	d_text_proc,	8,		176,	320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"No. de enemigos por pantalla",	NULL,	NULL	},
	{	d_box_proc,		8,		192,	40,		10,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_edit_proc,	9,		193,	38,		8,		BLACK,	WHITE,	0,		0,			6,		0,		input [6],	NULL,	NULL	},
	{	d_button_proc,	8,		216,	56,		16,		WHITE,	BLACK,	0,		D_EXIT,		0,		0,		"NUEVO",	NULL,	NULL	},
	{	d_text_proc,	8,		264,	320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"Abrir un proyecto existente",	NULL,	NULL	},
	{	d_box_proc,		8,		280,	320,	10,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_edit_proc,	9,		281,	318,	8,		BLACK,	WHITE,	0,		0,			128,	0,		input [7],	NULL,	NULL	},
	{	d_button_proc,	8,		304,	56,		16,		WHITE,	BLACK,	0,		D_EXIT,		0,		0,		"CARGAR",	NULL,	NULL	},
	
	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}
};

DIALOG numbicho[] = {
	{	d_box_proc,		128,	128,	128,	72,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	136,	136,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"No. Grafico",	NULL,	NULL	},
	{	d_text_proc,	136,	144,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"(1-3 en, 4 pm)",	NULL,	NULL	},
	{	d_edit_proc,	136,	160,	88,		8,		BLACK,	WHITE,	0,		0,			11,		0,		input [8],	NULL, 	NULL	},
	{	d_button_proc,	176,	176,	32,		16,		WHITE,	BLACK,	KEY_ENTER,	D_EXIT,		0,		0,		"OK", 		NULL,	NULL	},

	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}	
};

DIALOG numhotspot [] = {
	{	d_box_proc,		128,	128,	128,	72,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	136,	136,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"Tipo Hotspot",	NULL,	NULL	},
	{	d_text_proc,	136,	144,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"1 obj - 2 key",	NULL,	NULL	},
	{	d_edit_proc,	136,	160,	88,		8,		BLACK,	WHITE,	0,		0,			11,		0,		input [13],	NULL, 	NULL	},
	{	d_button_proc,	176,	176,	32,		16,		WHITE,	BLACK,	KEY_ENTER,	D_EXIT,		0,		0,		"OK", 		NULL,	NULL	},

	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}	       
};

DIALOG velocidadbicho[] = {
	{	d_box_proc,		128,	128,	128,	72,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	136,	136,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"Velocidad",	NULL,	NULL	},
	{	d_text_proc,	136,	144,	88,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"(1, 2, 4)",	NULL,	NULL	},
	{	d_edit_proc,	136,	160,	88,		8,		BLACK,	WHITE,	0,		0,			11,		0,		input [9],	NULL, 	NULL	},
	{	d_button_proc,	176,	176,	32,		16,		WHITE,	BLACK,	KEY_ENTER,	D_EXIT,		0,		0,		"OK", 		NULL,	NULL	},

	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}	
};

DIALOG pickafile[] = {
	{	d_box_proc,		128,	128,	336,	72,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	136,	136,	320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"Introduce un nombre de archivo",	NULL,	NULL	},
	{	d_text_proc,	136,	144,	320,	8,		WHITE,	BLACK,	0,		0,			0,		0,		"(con ruta o lo que quieras)",	NULL,	NULL	},
	{	d_edit_proc,	136,	160,	320,	8,		BLACK,	WHITE,	0,		0,			11,		0,		input [10],	NULL, 	NULL	},
	{	d_button_proc,	176,	176,	32,		16,		WHITE,	BLACK,	KEY_ENTER,	D_EXIT,		0,		0,		"OK", 		NULL,	NULL	},

	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}	
};

DIALOG modificabicho [] = {
	{	d_box_proc,		128,	128,	336,	72,		WHITE,	BLACK,	0,		0,			0,		0,		NULL,		NULL,	NULL	},
	{	d_text_proc,	136,	136,	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"Tipo",		NULL,	NULL	},
	{	d_edit_proc,	176,	136,	40,		8,		BLACK,	WHITE,	0,		0,			11,		0,		input [11],	NULL,	NULL	},
	{	d_text_proc,	296,	136,	40,		8,		WHITE,	BLACK,	0,		0,			0,		0,		"Vel.",		NULL,	NULL	},
	{	d_edit_proc,	336,	136,	40,		8,		BLACK,	WHITE,	0,		0,			11,		0,		input [12],	NULL,	NULL	},
	{	d_button_proc,	136,	176,	80,		16,		WHITE,	BLACK,	0,		D_EXIT,		0,		0,		"Cambiar",	NULL,	NULL	},
	{	d_button_proc,	296,	176,	80,		16,		WHITE,	BLACK,	0,		D_EXIT,		0,		0,		"Eliminar",	NULL,	NULL	},

	{	NULL,			0,		0,		0,		0,		0,		0,		0,		0,			0,		0,		NULL,		NULL,	NULL	}		
};

void loga (char *texto) {
	FILE *pf;
	
	pf = fopen ("log.txt", "a");
	fprintf (pf, "%s\n", texto);
	fclose (pf);	
}

void cleanup (void) {
	int i, j;
	int nscreens = header.map_w * header.map_h;
	
	loga ("cleanup - Vaciando estructuras");
	
	for (i = 0; i < nscreens; i ++) 
		for (j = 0; j < header.nenem; j ++) {
			tiestos [i][j].t = 0;
			tiestos [i][j].n = 0;
			tiestos [i][j].x = 0;
			tiestos [i][j].y = 0;
			tiestos [i][j].xx = 0;
			tiestos [i][j].yy = 0;
		}
		
	for (i =0; i < nscreens; i ++) {
        objs [i].x = objs [i].y = 0;
        objs [i].tipo = 0;    
	}
	
	loga ("cleanup - ok");
}

void recorta_tiles (BITMAP *tileset, BITMAP *tiles []) {
	int x = 0, y = 0;
	unsigned int index = 0;
	
	loga ("recorta_tiles - creando tile 0");

	tiles [0] = create_bitmap (16, 16);
	clear_to_color (tiles [0], 0);
	
	loga ("recorta_tiles - Recortando tiles");
	
	blit (tileset, screen, 0, 0, 0, 0, tileset -> w, tileset -> h);
	
	//allegro_message ("W: %i, H: %i", tileset->w, tileset->h);
	
	while (y <= tileset -> h - 16) {
		tiles [index] = create_bitmap (16, 16);
		blit (tileset, tiles [index++], x, y, 0, 0, 16, 16);
		x += 16;
		if (x > tileset -> w - 16) {
			x = 0;
			y += 16;
		}
	}
	
	maxt = index;
	//allegro_message ("Recortados %i tiles", maxt);
}

void save_data () {
	FILE *pf; 
	int i, j;
	int nscreens = header.map_w * header.map_h;
	
	// Diálogo filename (Filomena)
	popup_dialog (pickafile, 3);
	pf = fopen (input [10], "wb");
	input [10][0] = 0;
	
	// Primero la cabecera
	fwrite (&header, sizeof (MAPHEADER), 1, pf);
	
	// Luego todos los chunks
	for (i = 0; i < nscreens; i ++) 
		for (j = 0; j < header.nenem; j ++)
			fwrite (&tiestos [i][j], sizeof (ENEMYCHUNK), 1, pf);
			
	// Y por último todos los hotspots
	for (i =0 ; i < nscreens; i ++) 
		fwrite (&objs [i], sizeof (OBJPLACEMENT), 1, pf);
	
	// Cerramos y nos vamos
	fclose (pf);
}

void export_bin () {
    int i, j;
	int nscreens = header.map_w * header.map_h;	
	FILE *pf;
	int mx, my;
	int nenems = 0;
	unsigned char lsb, msb;
   
    popup_dialog (pickafile, 3);
	pf = fopen (input [10], "wb");
	input [10][0] = 0;
		
	for (i = 0; i < nscreens; i ++) 
		for (j = 0; j < header.nenem; j ++) {
			// Precalculamos mx y my para quitar problemas al programa destino
			mx = tiestos [i][j].n * signo (tiestos [i][j].xx - tiestos [i][j].x);
			my = tiestos [i][j].n * signo (tiestos [i][j].yy - tiestos [i][j].y);
			// 16 * (int)tiestos [i][j].x, 
            lsb = (16 * (unsigned char)tiestos [i][j].x) & 255;
			msb = (16 * (unsigned char)tiestos [i][j].x) >> 8;
            fputc (lsb, pf);
			fputc (msb, pf);
			// 16 * (int)tiestos [i][j].y, 
            lsb = (16 * (unsigned char)tiestos [i][j].y) & 255;
			msb = (16 * (unsigned char)tiestos [i][j].y) >> 8;
            fputc (lsb, pf);
			fputc (msb, pf);
			// 16 * (int)tiestos [i][j].x, 
			lsb = 16 * (unsigned char)tiestos [i][j].x;
			fputc (lsb, pf);
			// 16 * (int)tiestos [i][j].y, 
            lsb = 16 * (unsigned char)tiestos [i][j].y;
			fputc (lsb, pf);
            // 16 * (int)tiestos [i][j].xx, 
            lsb = 16 * (unsigned char)tiestos [i][j].xx;
			fputc (lsb, pf);
            // 16 * (int)tiestos [i][j].yy, 
            lsb = 16 * (unsigned char)tiestos [i][j].yy;
			fputc (lsb, pf);
            // mx, 
            lsb = (unsigned char)mx;
			fputc (lsb, pf);
            // my, 
            lsb = (unsigned char)my;
			fputc (lsb, pf);
            // tiestos [i][j].t
            lsb = (unsigned char)tiestos [i][j].t;
			fputc (lsb, pf);
		}
		
	for (i =0; i < nscreens; i ++) {
		// fprintf (pf, "\t{%d, %d, 0}", objs [i].xy, objs [i].tipo);
		lsb = (unsigned char) objs [i].x;
		fputc (lsb, pf);
		lsb = (unsigned char) objs [i].y;
		fputc (lsb, pf);
		lsb = (unsigned char) objs [i].tipo;
		fputc (lsb, pf);
		lsb = 0;
		fputc (lsb, pf);	
	}
	
	// Ar carao pipa
	fclose (pf);
}

void export_code () {
	int i, j;
	int nscreens = header.map_w * header.map_h;	
	FILE *pf;
	int mx, my;
	int nenems = 0;
/*
	// Hay que generar código C "ANSI" y z88dk-friendly
	// Para usar en huego y ezo isha.
	
	popup_dialog (pickafile, 3);
	pf = fopen (input [10], "w");
	input [10][0] = 0;
	
	// Primero vamos a realizar un conteo a ver cuántos enemigos
	// hay realmente en el mapa (con id < 4)
	for (i = 0; i < nscreens; i ++)  
	    for (j = 0; j < header.nenem; j ++)
            if (tiestos [i][j].t < 4)
               nenems ++;
	
	fprintf (pf, "#define BADDIES_COUNT %d\n\n", nenems);
	
	fputs ("typedef struct {\n", pf);
	fputs ("\tint x, y;\n", pf);
	fputs ("\tunsigned char x1, y1, x2, y2;\n", pf);
	fputs ("\tchar mx, my;\n", pf);
	fputs ("\tchar t;\n", pf);
    fputs ("#ifdef PLAYER_CAN_FIRE\n",pf);
    fputs ("\tunsigned char life;\n", pf);
    fputs ("#endif\n",pf);
    fputs ("} MALOTE;\n", pf);
	

	fputs ("\n", pf);
	fputs ("MALOTE malotes [] = {\n", pf);
	for (i = 0; i < nscreens; i ++) 
		for (j = 0; j < header.nenem; j ++) {
			// Precalculamos mx y my para quitar problemas al programa destino
			mx = tiestos [i][j].n * signo (tiestos [i][j].xx - tiestos [i][j].x);
			//allegro_message ("mx=%d => n=%d, xx=%d, x=%d, sgn=%d", mx, tiestos[i][j].n, tiestos[i][j].xx, tiestos[i][j].x, signo (tiestos [i][j].xx - tiestos [i][j].x));
			my = tiestos [i][j].n * signo (tiestos [i][j].yy - tiestos [i][j].y);
			fprintf(pf, "\t{%d, %d, %d, %d, %d, %d, %d, %d, %d}", 16 * (int)tiestos [i][j].x, 16 * (int)tiestos [i][j].y, 16 * (int)tiestos [i][j].x, 16 * (int)tiestos [i][j].y, 16 * (int)tiestos [i][j].xx, 16 * (int)tiestos [i][j].yy, mx, my, tiestos [i][j].t);
			if (i == nscreens - 1 && j == header.nenem - 1)
				fprintf (pf, "\n");
			else
				fprintf (pf, ",\n");
		}
	fputs ("};\n\n", pf);
	
	fputs ("typedef struct {\n", pf);
	fputs ("\tunsigned char xy, tipo, act;\n", pf);
	fputs ("} HOTSPOT;\n\n", pf);
	fputs ("HOTSPOT hotspots [] = {\n", pf);
	for (i =0; i < nscreens; i ++) {
		fprintf (pf, "\t{%d, %d, 0}", objs [i].xy, objs [i].tipo);
		if (i == nscreens - 1)
				fprintf (pf, "\n");
			else
				fprintf (pf, ",\n");
	}
	fputs ("};\n\n", pf);
	
	// Ar carao pipa
	fclose (pf);
*/
}

void export_txt () {
    int i, j;
	int nscreens = header.map_w * header.map_h;	
	FILE *pf;
	int mx, my;
	int nenems = 0;

	// Genera ENEMS.bex y HOTSPOTS.bex
	
	pf = fopen ("enems.bex", "w");
	fprintf(pf,"\t' Data t, x1, y1, x2, y2, v\n");
	fprintf(pf,"enemy_data:\n");
	
	for (i = 0; i < nscreens; i ++)  {
        fprintf (pf, "\tdata ");
		for (j = 0; j < header.nenem; j ++) {
			// Precalculamos mx y my para quitar problemas al programa destino
			mx = tiestos [i][j].n * signo (tiestos [i][j].xx - tiestos [i][j].x);
			//allegro_message ("mx=%d => n=%d, xx=%d, x=%d, sgn=%d", mx, tiestos[i][j].n, tiestos[i][j].xx, tiestos[i][j].x, signo (tiestos [i][j].xx - tiestos [i][j].x));
			my = tiestos [i][j].n * signo (tiestos [i][j].yy - tiestos [i][j].y);
			//fprintf(pf, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 16 * (int)tiestos [i][j].x, 16 * (int)tiestos [i][j].y, 16 * (int)tiestos [i][j].x, 16 * (int)tiestos [i][j].y, 16 * (int)tiestos [i][j].xx, 16 * (int)tiestos [i][j].yy, mx, my, tiestos [i][j].t/*, 1 + (rand () % 7)*/);
			fprintf(pf,"%d, %d, %d, %d, %d, %d", tiestos [i][j].t, tiestos [i][j].x, tiestos [i][j].y, tiestos [i][j].xx, tiestos [i][j].yy, tiestos [i][j].n);
			if (tiestos[i][j].t != 0 && tiestos [i][j].t != 8) nenems ++;
			if (j < header.nenem - 1) fprintf (pf, ", ");
		}
		fprintf (pf, "\n");
    }
    fprintf (pf, "' total: %d", nenems);
    fclose (pf);
    
    pf = fopen ("hotspots.bex", "w");
		fprintf (pf, "' For each screen, Data x, y\n");
	    for (i =0; i < nscreens; i ++) {
		//fprintf (pf, "%d,%d,%d,0\n", objs [i].x, objs [i].y, objs [i].tipo);
		fprintf (pf, "Data %d, %d\n", objs [i].x, objs [i].y);
	}
    fclose (pf);	
}

void draw_screen (BITMAP *buffer, char map_x, char map_y, BITMAP *tiles []) {
	int i, x, y;
	FILE *pf;
	unsigned char c; 
	
	loga ("draw_screen - Abriendo el archivo");
	
	pf = fopen (header.fname_map, "rb");
	
	loga ("draw_screen - Leyendo y pintando");
	
	scare_mouse ();
	
	for (x = 0; x < header.scr_w; x ++)
		for (y = 0; y < header.scr_h; y ++) {
			fseek ( pf, (header.scr_w * header.map_w) * (header.scr_h * map_y + y) + 		// coordenada Y
			        	(header.scr_w * map_x + x), 										// coordenada X
					SEEK_SET );
			c = fgetc (pf);
			
			if (c < maxt)
				stretch_blit (tiles [c], buffer, 0, 0, 16, 16, x << 5, 8 + (y << 5), 32, 32);
				
			hline (buffer, x << 5, 8 + (y << 5), 31 + (x << 5), makecol (100, 100, 100));
			vline (buffer, x << 5, 8 + (y << 5), 39 + (y << 5), makecol (100, 100, 100));
		}
	
	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), 0, "SCR px = %d[%d], py = %d[%d]        ", map_x, header.map_w, map_y, header.map_h);
	
	// Pinto los enemigos
	for (i = 0; i < header.nenem; i ++)
		if (tiestos [map_x + map_y * header.map_w][i].t != 0) {
			rect (buffer, 8 + (tiestos [map_x + map_y * header.map_w][i].x << 5), 16 + (tiestos [map_x + map_y * header.map_w][i].y << 5), 24 + (tiestos [map_x + map_y * header.map_w][i].x << 5), 32 + (tiestos [map_x + map_y * header.map_w][i].y << 5), makecol (100, 255, 100));
			rect (buffer, 8 + (tiestos [map_x + map_y * header.map_w][i].xx << 5), 16 + (tiestos [map_x + map_y * header.map_w][i].yy << 5), 24 + (tiestos [map_x + map_y * header.map_w][i].xx << 5), 32 + (tiestos [map_x + map_y * header.map_w][i].yy << 5), makecol (100, 255, 100));
			line (buffer, 16 + (tiestos [map_x + map_y * header.map_w][i].x << 5), 24 + (tiestos [map_x + map_y * header.map_w][i].y << 5), 16 + (tiestos [map_x + map_y * header.map_w][i].xx << 5), 24 + (tiestos [map_x + map_y * header.map_w][i].yy << 5), makecol (100, 255, 100));
			textprintf_ex (buffer, font, (tiestos [map_x + map_y * header.map_w][i].x << 5) + 12, (tiestos [map_x + map_y * header.map_w][i].y << 5) + 20, makecol (255, 255, 100), 0, "%d", tiestos [map_x + map_y * header.map_w][i].t);
		}
		
	// Pinto el hotspot
	x = objs [map_x + map_y * header.map_w].x;//y >> 4;
	y = objs [map_x + map_y * header.map_w].y;// & 15;
	rect (buffer, 8 + (x << 5), 16 + (y << 5), 24 + (x << 5), 32 + (y << 5), makecol (100,100,255));
	line (buffer, 8 + (x << 5), 16 + (y << 5), 24 + (x << 5), 32 + (y << 5), makecol (100,100,255));
	line (buffer, 8 + (x << 5), 32 + (y << 5), 24 + (x << 5), 16 + (y << 5), makecol (100,100,255));
	textprintf_ex (buffer, font, 12 + (x << 5), 20 + (y << 5), makecol (255, 255, 100), 0, "%d", objs [map_x + map_y * header.map_w].tipo);
	
	show_mouse (buffer);
		
	loga ("draw_screen - Cerrando el archivo");
		
	fclose (pf);
	
	loga ("draw_screen - saliendo");
}

void editor (void) {
	BITMAP *tileset;
	BITMAP *tiles[256];		// Otra vez de bejtia. Pero suficiente.
	BITMAP *buffer;
	char map_x = 0;
	char map_y = 0;
	char bicho;
	int i, j;
	ENEMYCHUNK enemigo_temp;
	char enemyplaced;
	char queboton;
	
	loga ("editor - inicializando");
	
	// Creamos los bitmaps
	
	buffer = create_bitmap (640, 480);
	tileset = load_bitmap (header.fname_til, NULL);
	
	show_mouse (buffer);
	
	recorta_tiles (tileset, tiles);
	
	draw_screen (buffer, map_x, map_y, tiles);
	blit (buffer,screen,0,0,0,0,640,480);
		
	while (!key [KEY_ESC]) {
		if (key [KEY_LEFT] && map_x > 0) {
			map_x --;
			draw_screen (buffer, map_x, map_y, tiles);
			while (key [KEY_LEFT]);
		} else if (key [KEY_RIGHT] && map_x < header.map_w - 1) {
			map_x ++;
			draw_screen (buffer, map_x, map_y, tiles);
			while (key [KEY_RIGHT]);
		}
		
		if (key [KEY_UP] && map_y > 0) {
			map_y --;
			draw_screen (buffer, map_x, map_y, tiles);
			while (key [KEY_UP]);
		} else if (key [KEY_DOWN] && map_y < header.map_h - 1) {
			map_y ++;
			draw_screen (buffer, map_x, map_y, tiles);
			while (key [KEY_DOWN]);
		}
		
		if (key [KEY_S]) {
			// guardar datos
			while (key [KEY_S]);
			show_mouse (NULL);
			blit (buffer, screen, 0, 0, 0, 0, 640, 480);
			save_data ();
			show_mouse (buffer);
		}
		
		if (key [KEY_E]) {
			// exportar código
			while (key [KEY_E]);
			show_mouse (NULL);
			blit (buffer, screen, 0, 0, 0, 0, 640, 480);
			export_code ();
			show_mouse (buffer);
		}
		
		if (key [KEY_B]) {
            // exportar BINARIO
            while (key [KEY_B]);
            show_mouse (NULL);
            blit (buffer, screen, 0, 0, 0, 0, 640, 480);
			export_bin ();
			show_mouse (buffer);
        }
        
        if (key [KEY_T]) {
            // exportar PLAINTEXT
            while (key [KEY_T]);
            show_mouse (NULL);
            blit (buffer, screen, 0, 0, 0, 0, 640, 480);
            export_txt ();
            show_mouse (buffer);   
        }
		
		if (mouse_b & 2) {
			//objs [map_x + map_y * header.map_w].xy = ((mouse_x >> 5)<<4) + ((mouse_y - 8) >> 5);
			objs [map_x + map_y * header.map_w].x = (mouse_x >> 5);
			objs [map_x + map_y * header.map_w].y = ((mouse_y - 8) >> 5);
            popup_dialog (numhotspot, 3);
			objs [map_x + map_y * header.map_w].tipo = atoi (input [13]);
			show_mouse (NULL);
			draw_screen (buffer, map_x, map_y, tiles);
			show_mouse (buffer);
		}
		
		if (mouse_b & 1) {
			// Vemos si la casilla tiene o no bicho
			enemigo_temp.x = mouse_x >> 5;
			enemigo_temp.y = (mouse_y - 8) >> 5;
				
			bicho = 0;
			for (i = 0; i < header.nenem && !bicho; i ++)
				if (tiestos [map_x + map_y * header.map_w][i].t != 0)
					if (tiestos [map_x + map_y * header.map_w][i].x == enemigo_temp.x && tiestos [map_x + map_y * header.map_w][i].y == enemigo_temp.y)
						bicho = 1;
				
			if (!bicho) {
				// Primero hay que ver que quepan más bichos en la pantalla
				
				j = 0;
				for (i = 0; i < header.nenem; i ++)
					if (tiestos [map_x + map_y * header.map_w][i].t != 0)
					   j ++;
				
				if (j < header.nenem) {
					// Poner un bicho nuevo
					
					// Ahora que sabemos qué bicho poner, marcamos la casilla
					show_mouse (NULL);
					rectfill (buffer, (enemigo_temp.x << 5) + 8, (enemigo_temp.y << 5) + 16, (enemigo_temp.x << 5) + 24, (enemigo_temp.y << 5) + 32, makecol (255, 100, 100));
					blit (buffer, screen, 0, 0, 0, 0, 640, 480);
					show_mouse (buffer); 
					
					popup_dialog (numbicho, 3);
					enemigo_temp.t = atoi (input [8]);
					input [8][0] = 0;
					
					show_mouse (NULL);
					textprintf_ex (buffer, font, (enemigo_temp.x << 5) + 12, (enemigo_temp.y << 5) + 20, makecol (255, 255, 100), 0, "%d", enemigo_temp.t);
					show_mouse (buffer); 
					
					// Ahora hay que marcar la segunda coordenada
					enemyplaced = 0;
					while (!enemyplaced) {
						if (mouse_b) {
							enemigo_temp.xx = mouse_x >> 5;
							enemigo_temp.yy = (mouse_y - 8) >> 5;
							
							show_mouse (NULL);
							line (buffer, 16 + (enemigo_temp.x << 5), 24 + (enemigo_temp.y << 5), 16 + (enemigo_temp.xx << 5), 24 + (enemigo_temp.yy << 5), makecol (255, 100, 100));
							rectfill (buffer, (enemigo_temp.xx << 5) + 8, (enemigo_temp.yy << 5) + 16, (enemigo_temp.xx << 5) + 24, (enemigo_temp.yy << 5) + 32, makecol (255, 100, 100));
							blit (buffer, screen, 0, 0, 0, 0, 640, 480);
							show_mouse (buffer);
							
							// mostramos el diálogo para la velocidad:
							popup_dialog (velocidadbicho, 3);
							enemigo_temp.n = atoi (input [9]);
							input [9][0] = 0;
							
							// Buscamos un slot libre
							for (i = 0; i < header.nenem; i ++)
								if (tiestos [map_x + map_y * header.map_w][i].t == 0) {
									tiestos [map_x + map_y * header.map_w][i].t = enemigo_temp.t;
									tiestos [map_x + map_y * header.map_w][i].x = enemigo_temp.x;
									tiestos [map_x + map_y * header.map_w][i].y = enemigo_temp.y;
									tiestos [map_x + map_y * header.map_w][i].xx = enemigo_temp.xx;
									tiestos [map_x + map_y * header.map_w][i].yy = enemigo_temp.yy;
									tiestos [map_x + map_y * header.map_w][i].n = enemigo_temp.n;
									
									textprintf_ex (buffer, font, 0, 450, makecol (255, 255, 255), 0, "%d-> %d (%d, %d) - (%d, %d) @ %d                    ", i, enemigo_temp.t, enemigo_temp.x, enemigo_temp.y, enemigo_temp.xx, enemigo_temp.yy, enemigo_temp.n);
									
									i = header.nenem;
								}
								
							// Finito:
							enemyplaced = 1;
						}
						
						if (key [KEY_ESC]) {
							// Cancelar
							
							draw_screen (buffer, map_x, map_y, tiles);
							enemyplaced = 1;
							while (key [KEY_ESC]);
						}
						
						vsync ();
						blit (buffer, screen, 0, 0, 0, 0, 640, 480);	
					}
				}
			} else {
				// Quitar o modificar el bicho, muestro diálogo (qué me gucha)
				
				// Vemos qué bicho hemos pulsado:
				bicho = -1;
				
				for (i = 0; i < header.nenem && bicho == -1; i ++)
					if (tiestos [map_x + map_y * header.map_w][i].t != 0)
						if (tiestos [map_x + map_y * header.map_w][i].x == enemigo_temp.x && tiestos [map_x + map_y * header.map_w][i].y == enemigo_temp.y)
							bicho = i;

				// Hay que modificar el bicho i.
				// Vamos a presentar el diálogo, primero lo rellenamos:
					
				sprintf (input [11], "%d", tiestos [map_x + map_y * header.map_w][bicho].t);
				sprintf (input [12], "%d", tiestos [map_x + map_y * header.map_w][bicho].n);
				
				// Mostrar diálogo:
				show_mouse (NULL);
				blit (buffer, screen, 0, 0, 0, 0, 640, 480);
				show_mouse (buffer);
				queboton = popup_dialog (modificabicho, 2);
				
				if (queboton == 6) {
					// Borrar
					tiestos [map_x + map_y * header.map_w][bicho].t = 0;
					draw_screen (buffer, map_x, map_y, tiles);
				} else {
					// Modificar
					tiestos [map_x + map_y * header.map_w][bicho].t = atoi (input [11]);
					tiestos [map_x + map_y * header.map_w][bicho].n	= atoi (input [12]);
					
					show_mouse (NULL);
					textprintf_ex (buffer, font, (enemigo_temp.x << 5) + 12, (enemigo_temp.y << 5) + 20, makecol (255, 255, 100), 0, "%d", tiestos [map_x + map_y * header.map_w][bicho].t);
					show_mouse (buffer); 
				}
			}
		}
		
		
		textprintf_ex (buffer, font, 220, 0, makecol (255, 255, 255), 0, "TX=%d ", mouse_x>>5);
		textprintf_ex (buffer, font, 280, 0, makecol (255, 255, 255), 0, "TY=%d ", (mouse_y-8)>>5);
		
		vsync ();
		blit (buffer, screen, 0, 0, 0, 0, 640, 480);
	}
	
	loga ("editor - destruyendo bitmaps");
	
	show_mouse (NULL);
	
	destroy_bitmap (buffer);
	destroy_bitmap (tileset);	
	
	loga ("editor - saliendo");
}

int main (void) {
	int i, j;
	int queboton;
	int nscreens; 
	FILE *pf;
		
	allegro_init ();
	install_keyboard ();
	install_mouse ();
	install_timer ();

	set_color_depth (32);
	if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
		set_color_depth (16);
		set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	}
	set_palette (desktop_palette);

	// Soy un cutre de la hostia, pero es que *no tengo ganas* :P

	for (i = 0; i < 25; i ++) {
		if (entrada	[i].bg == BLACK)
			entrada [i].bg = makecol (100, 100, 100);
		else
			entrada [i].bg = makecol (255, 255, 200);
			
		if (entrada [i].fg == WHITE)
			entrada [i].fg = makecol (255, 255, 255);
		else
			entrada [i].fg = makecol (10, 10, 40);
	}
	
	for (i = 0; i < 5; i ++) {
		if (numbicho [i].bg == BLACK)
			numbicho [i].bg = makecol (100, 100, 100);
		else
			numbicho [i].bg = makecol (255, 255, 200);
			
		if (numbicho [i].fg == WHITE)
			numbicho [i].fg = makecol (255, 255, 255);
		else
			numbicho [i].fg = makecol (10, 10, 40);
			
		if (velocidadbicho [i].bg == BLACK)
			velocidadbicho [i].bg = makecol (100, 100, 100);
		else
			velocidadbicho [i].bg = makecol (255, 255, 200);
			
		if (velocidadbicho [i].fg == WHITE)
			velocidadbicho [i].fg = makecol (255, 255, 255);
		else
			velocidadbicho [i].fg = makecol (10, 10, 40);

		if (pickafile [i].bg == BLACK)
			pickafile [i].bg = makecol (100, 100, 100);
		else
			pickafile [i].bg = makecol (255, 255, 200);
			
		if (pickafile [i].fg == WHITE)
			pickafile [i].fg = makecol (255, 255, 255);
		else
			pickafile [i].fg = makecol (10, 10, 40);
			
		if (numhotspot [i].bg == BLACK)
			numhotspot [i].bg = makecol (100, 100, 100);
		else
			numhotspot [i].bg = makecol (255, 255, 200);
			
		if (numhotspot [i].fg == WHITE)
			numhotspot [i].fg = makecol (255, 255, 255);
		else
			numhotspot [i].fg = makecol (10, 10, 40);
			
	}

	for (i = 0; i < 7; i ++) {
		if (modificabicho [i].bg == BLACK)
			modificabicho [i].bg = makecol (100, 100, 100);
		else
			modificabicho [i].bg = makecol (255, 255, 200);
			
		if (modificabicho [i].fg == WHITE)
			modificabicho [i].fg = makecol (255, 255, 255);
		else
			modificabicho [i].fg = makecol (10, 10, 40);
	}
	
	// El dialogaso:
	
	queboton = do_dialog (entrada, -1);
	
	if (queboton == 20)	{
		// Nuevo
		
		// Primero rellenamos la cabecera
		strcpy (header.fname_map, input [0]);
		strcpy (header.fname_til, input [1]);
		
		header.map_w = atoi (input [2]);
		header.map_h = atoi (input [3]);
		header.scr_w = atoi (input [4]);
		header.scr_h = atoi (input [5]);
		
		header.nenem = atoi (input [6]);
		
		// Ahora inicializamos todos los tiestos
		cleanup ();
		
		// Llamamos al editor
		editor ();
		
	} else if (queboton == 24) {
		// Cargar

		// Primero cargamos los tiestos
		pf = fopen (input [7], "rb");
		
		// Primero la cabecera
		fread (&header, sizeof (MAPHEADER), 1, pf);
		nscreens = header.map_w * header.map_h;	
		
		// Luego todos los chunks
		for (i = 0; i < nscreens; i ++) 
			for (j = 0; j < header.nenem; j ++)
				fread (&tiestos [i][j], sizeof (ENEMYCHUNK), 1, pf);
				
		// Y por último todos los hotspots
		for (i =0 ; i < nscreens; i ++) 
			fread (&objs [i], sizeof (OBJPLACEMENT), 1, pf);
		
		// Cerramos y nos vamos
		fclose (pf);
		
		// Llamamos al editor
		editor ();		
	} else {
		// Pulsamos ESC
		
		return 0;	
	}
}
END_OF_MAIN();
