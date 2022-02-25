// Functions Involved !
// For master
void TWIInit(void)
{
    // Set Baud Rate Register
    TWSR = 0x00;
    TWBR = 0x0C;
    
    //enable TWI(I2C)
    TWCR = (1<<TWEN);
}

void TWIStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0); // Wait till start condition is executed !
}


void TWI_Write_Addr(unsigned char Addr){
    TWDR = Addr;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(TWCR & (1<<TWINT) == 0);
    while(TWSR & (0XF8) != 0x18);  //Check for the acknowledgement !
    // TWSR & 0xF8 This represents condition for acknowledgement
    //  For address sending, it is 0x18
    // For data sending, it is 0x28
}

// Write_Data function can be same mentioned in PDF !

void TWI_Stop(){
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while(TWCR & (1<<TWINT) == 0);   //Wait till stop condition is transmitted !
}

// Correction in main function is it's not PINA0 and PINA1, it should be PA0 and PA1 

//For Slave !
// Remove acknowledgement code from read_data function !