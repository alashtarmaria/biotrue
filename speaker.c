#define SPEAKER_PIN     LATGbits.LATG9
#define SPEAKER_TRIS    TRISGbits.TRISG9

void Speaker_Init() {
    SPEAKER_TRIS = 0;
    SPEAKER_PIN = 0;
}

void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);
    PPS_Mapping(68, _OUTPUT, _U3TX);
    UART3_Init(baudrate);
    Delay_ms(100);
}

void Speaker_Beep_Short() {
    SPEAKER_PIN = 1;
    Delay_ms(300);
    SPEAKER_PIN = 0;
    Delay_ms(150);
}

void Speaker_DoubleBeep() {
    Speaker_Beep_Short();
    Speaker_Beep_Short();

}

void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    UART3_Init_Config(9600);
    Speaker_Init();

    UART3_Write_Text("Kod yuklendi!\r\n");
    Speaker_DoubleBeep();

    while (1) {

    }
}
