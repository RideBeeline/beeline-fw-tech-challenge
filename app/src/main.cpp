

#include "velo2_home.cpp"

void ui_render()
{
    draw_velo2_home();
}

void setup()
{
    lcd_init();
}

void loop()
{   
    lcd_refresh(ui_render);
}

// this would run on the device but when we use the python emulator, it calls setup() and loop() directly
int main() {
    setup();
    while(true){
        loop();
    }
    return 0;
}