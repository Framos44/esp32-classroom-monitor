// =====================================================================
// SIMAEP - Caixa para impressão 3D (ESP32 + LCD 20x4 I2C + sensores)
// Duas peças: base (fundo + paredes) e tampa frontal.
// Todas as medidas em mm. Confira com paquímetro e ajuste os parâmetros.
// =====================================================================

/* [Qual peça renderizar] */
// "print" = base e tampa lado a lado, prontas pra fatiar
// "base" / "lid" = só uma peça | "assembled" = montada (só pra ver)
part = "print";

/* [Caixa] */
outer_w  = 120;   // largura externa
outer_h  = 112;   // altura externa
base_d   = 46;    // profundidade da base
wall     = 2.4;   // parede (multiplo de 0.4 pra bico de 0.4)
corner_r = 4;     // raio dos cantos
plate_t  = 3;     // espessura da tampa frontal

/* [LCD 20x4 - CONFIRA NO SEU MODULO] */
lcd_pcb        = [98, 60];     // tamanho da placa
lcd_hole_sp    = [93, 55];     // distancia entre furos de fixacao
lcd_view       = [77.5, 27];   // janela (area visivel 76 x 25 + folga)
lcd_view_dy    = 0;            // desloca a janela pra cima(+)/baixo(-)
lcd_standoff_h = 10;           // altura do pilar (frente da placa ate o vidro/moldura)
lcd_cx = 0;
lcd_cy = 18;                   // centro do LCD (vertical) na tampa

/* [ESP32 DevKit] */
esp_size    = [28.6, 52];      // largura x comprimento da placa
esp_cy      = 16;              // posicao vertical do centro
esp_gap     = 0.5;
esp_wall    = 1.6;
esp_frame_h = 10;

/* [LEDs, LDR e microfone (fileira na parte de baixo da tampa)] */
strip_y   = -26;
led_d     = 5.2;               // LED 5 mm
led_xs    = [-42, -22, -2];
ldr_x     = 20;  ldr_d = 5.4;  // LDR 5 mm
mic_x     = 42;  mic_d = 4;    // furo de som do KY-038
engrave_labels = true;

/* [Ventilacao e cabo] */
vent_w = 3;  vent_z0 = 8;  vent_len = 24;
dht_vent_xs = [-50, -44, -38, -32, -26];  // base, lado do DHT22
mq_vent_xs  = [ 26,  32,  38,  44,  50];  // base e topo, lado do MQ135
usb_w = 14;  usb_z0 = 4;  usb_h = 10;
rib_x = 20;  rib_h = 30;                   // divisorias que isolam o DHT22 do calor do MQ135

/* [Fixacao] */
post_pos = [[54, 50], [-54, 50], [54, -50], [-54, -50]];
key_x = 30;  key_y = 35;                   // furos de fechadura (pendurar na parede)

$fn = 48;

// ---------------------------------------------------------------------
module rrect(w, h, r, t) {
    linear_extrude(t) offset(r = r) square([w - 2*r, h - 2*r], center = true);
}

module keyhole() {
    // cabeca larga em cima, ranhura pra baixo (a caixa "desce" no parafuso)
    cylinder(d = 9, h = wall + 2);
    hull() {
        cylinder(d = 4.5, h = wall + 2);
        translate([0, -8, 0]) cylinder(d = 4.5, h = wall + 2);
    }
}

module esp_frame() {
    ow = esp_size[0] + 2*(esp_gap + esp_wall);
    oh = esp_size[1] + 2*(esp_gap + esp_wall);
    translate([0, esp_cy, wall - 0.01]) difference() {
        translate([-ow/2, -oh/2, 0]) cube([ow, oh, esp_frame_h]);
        translate([-esp_size[0]/2 - esp_gap, -esp_size[1]/2 - esp_gap, -0.1])
            cube([esp_size[0] + 2*esp_gap, esp_size[1] + 2*esp_gap, esp_frame_h + 1]);
        // saida do USB (lado de baixo)
        translate([-8, -oh/2 - 1, 2]) cube([16, esp_wall + 2, esp_frame_h]);
        // recortes pra pegar a placa com o dedo
        for (sx = [-1, 1])
            translate([sx*(ow/2 - esp_wall/2) - (esp_wall + 2)/2, -6, 3])
                cube([esp_wall + 2, 12, esp_frame_h]);
    }
}

module base() {
    difference() {
        union() {
            difference() {
                rrect(outer_w, outer_h, corner_r, base_d);
                translate([0, 0, wall])
                    rrect(outer_w - 2*wall, outer_h - 2*wall, max(0.5, corner_r - wall), base_d);
            }
            // pilares dos parafusos da tampa (M3 autorroscante)
            for (p = post_pos) translate([p[0], p[1], 0]) difference() {
                translate([0, 0, wall - 0.01]) cylinder(d = 7, h = base_d - wall + 0.01);
                translate([0, 0, base_d - 12]) cylinder(d = 2.6, h = 12.1);
            }
            // divisorias entre DHT22 (esq.), cabo USB (centro) e MQ135 (dir.)
            for (sx = [-1, 1])
                translate([sx*rib_x - 0.8, -outer_h/2 + wall - 0.01, wall - 0.01])
                    cube([1.6, 40, rib_h]);
            esp_frame();
        }
        // ventilacao: base (DHT22 e MQ135) e topo (saida do ar quente do MQ135)
        for (x = concat(dht_vent_xs, mq_vent_xs))
            translate([x - vent_w/2, -outer_h/2 - 1, vent_z0]) cube([vent_w, wall + 2, vent_len]);
        for (x = mq_vent_xs)
            translate([x - vent_w/2, outer_h/2 - wall - 1, vent_z0]) cube([vent_w, wall + 2, vent_len]);
        // saida do cabo USB
        translate([-usb_w/2, -outer_h/2 - 1, usb_z0]) cube([usb_w, wall + 2, usb_h]);
        // furos de fechadura nas costas
        for (sx = [-1, 1]) translate([sx*key_x, key_y, -1]) keyhole();
    }
}

// Tampa desenhada na posicao de montagem: face externa em z = plate_t, pilares do LCD pra baixo.
module lid() {
    difference() {
        union() {
            rrect(outer_w, outer_h, corner_r, plate_t);
            for (sx = [-1, 1], sy = [-1, 1])
                translate([lcd_cx + sx*lcd_hole_sp[0]/2, lcd_cy + sy*lcd_hole_sp[1]/2, -lcd_standoff_h])
                    cylinder(d = 6, h = lcd_standoff_h + 0.01);
        }
        // furos M3 nos pilares do LCD
        for (sx = [-1, 1], sy = [-1, 1])
            translate([lcd_cx + sx*lcd_hole_sp[0]/2, lcd_cy + sy*lcd_hole_sp[1]/2, -lcd_standoff_h - 0.1])
                cylinder(d = 2.6, h = 8);
        // janela do LCD
        translate([lcd_cx - lcd_view[0]/2, lcd_cy + lcd_view_dy - lcd_view[1]/2, -1])
            cube([lcd_view[0], lcd_view[1], plate_t + 2]);
        // LEDs, LDR, microfone
        for (x = led_xs) translate([x, strip_y, -1]) cylinder(d = led_d, h = plate_t + 2);
        translate([ldr_x, strip_y, -1]) cylinder(d = ldr_d, h = plate_t + 2);
        translate([mic_x, strip_y, -1]) cylinder(d = mic_d, h = plate_t + 2);
        // parafusos da tampa (com rebaixo)
        for (p = post_pos) {
            translate([p[0], p[1], -1]) cylinder(d = 3.4, h = plate_t + 2);
            translate([p[0], p[1], plate_t - 1.5]) cylinder(h = 1.51, d1 = 3.4, d2 = 6.4);
        }
        // legendas gravadas
        if (engrave_labels) {
            labels = ["Ok", "Atenção", "Alerta"];
            for (i = [0 : 2])
                translate([led_xs[i], strip_y - 8, plate_t - 0.4])
                    linear_extrude(0.5) text(labels[i], size = 3, halign = "center", valign = "center");
            translate([ldr_x, strip_y - 8, plate_t - 0.4])
                linear_extrude(0.5) text("LDR", size = 2.8, halign = "center", valign = "center");
            translate([mic_x, strip_y - 8, plate_t - 0.4])
                linear_extrude(0.5) text("Mic", size = 2.8, halign = "center", valign = "center");
        }
    }
}

// Tampa virada com a face externa na mesa de impressao
module lid_print() {
    translate([0, 0, plate_t]) rotate([180, 0, 0]) lid();
}

if (part == "base") base();
else if (part == "lid") lid_print();
else if (part == "assembled") { base(); translate([0, 0, base_d]) lid(); }
else { base(); translate([outer_w + 10, 0, 0]) lid_print(); }
