// uart library 
// perpheral pin select library 
// rx - tx 
// tx - rx 
//baudrate 9600 
void main() {
    PLLFBD = 70;            // PLL multiplier M=70
    CLKDIV = 0x0000;     // PLL prescaler N1=2, PLL postscaler N2=2

    TRISB.B6 = 0;  // RB6 çikis olarak ayarlandi
    LATB.B6 = 0;   // Baslangiçta LED kapali


    PPS_Mapping(71, _INPUT,  _U4RX);  // UART2 RX için RP66 (RD2)
    PPS_Mapping(70, _OUTPUT, _U4TX);  // UART2 TX için RP67 (RD3)

    UART4_Init(9600);  // 9600 baud hizinda UART baslat
    Delay_ms(1000);  // UART modülünün stabilize olmasi için bekle

    UART4_Write_Text("start testing");  // Terminale mesaj gönder
    Delay_ms(1000);

    while (1) {
        LATB.B6 = 1; // RB6 (PORTB 6. LED) yak
        Delay_ms(1000);
        UART4_Write_Text("LED YANDI\r\n");


        LATB.B6 = 0; // RB6 (PORTB 6. LED) söndür
        Delay_ms(1000);
        UART4_Write_Text("LED SONDU\r\n");

    }
}
