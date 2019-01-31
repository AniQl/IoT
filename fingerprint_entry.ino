#include <SoftwareSerial.h>
#include <FPS_GT511C3.h>

int enroll_state = 0;
int entry_state = 0;
int received = 0;
FPS_GT511C3 fps(4,5);
int x = 1;
int z = 1;

void setup() {

    Serial.begin(9600);
    pinMode(7,OUTPUT);

}

void loop() {

    if(Serial.available()>0)
    {
        received = Serial.read();
        Serial.println(received);
    }

// enroll

    if (enroll_state == 0 && received == '1')
    {
        delay(100);
        fps.Open();
        fps.SetLED(true);
        Enroll();
    }

//entry

    if (entry_state == 0 && received == '2')
    {
        delay(100);
        fps.Open();

        //while(fps.IsPressFinger() == false){}

        fps.SetLED(true);
        entry();
    }
// entry_state = 0;
}

void Enroll() {
    // Enroll test
    // find open enroll id
    int enrollid = x;
    fps.EnrollStart(enrollid);

    // enroll
    Serial.print("Press finger to Enroll #");
    Serial.println(enrollid);
    while(fps.IsPressFinger() == false) delay(100);
    bool bret = fps.CaptureFinger(true);
    int iret = 0;
    if (bret != false)
    {
        Serial.println("Remove finger");
        fps.Enroll1();
        while(fps.IsPressFinger() == true) delay(100);
        Serial.println("Press same finger again");
        while(fps.IsPressFinger() == false) delay(100);
        bret = fps.CaptureFinger(true);
        if (bret != false)
        {
            Serial.println("Remove finger");
            fps.Enroll2();
            while(fps.IsPressFinger() == true) delay(100);
            Serial.println("Press same finger yet again");
            while(fps.IsPressFinger() == false) delay(100);
            bret = fps.CaptureFinger(true);
            if (bret != false)
            {
                Serial.println("Remove finger");
                iret = fps.Enroll3();
                if (iret == 0)
                {
                    Serial.println("Enrolling Successfull");
                    x = ++x;
                    delay(1000);

                }
                else
                {
                    Serial.print("Enrolling Failed with error code:");
                    Serial.println(iret);
                }
            }
            else Serial.println("Failed to capture third finger");
        }
        else Serial.println("Failed to capture second finger");
    }
    else Serial.println("Failed to capture first finger");
    //enroll_state =0;
    received='2';
}

void entry() {
    if (fps.IsPressFinger())
    {
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
        if (id == 0 || id == 1 || id == 2 || id == 3 || id == 4 || id == 5 )
        {
            digitalWrite(7, HIGH);
            delay(2000);
            digitalWrite(7, LOW);
            Serial.println(id);
        }
        else
        {
            Serial.println("Finger not found");
        }
    }
    else
    {
        Serial.println("Please press finger");
    }
}
