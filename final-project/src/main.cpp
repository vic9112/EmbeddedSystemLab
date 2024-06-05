#include "mbed.h"
#include "bbcar.h"

#include <events/mbed_events.h>
#include "ble/BLE.h"
#include "gatt_server_process.h"
#include "mbed-trace/mbed_trace.h"
#include "accelerometer.h"
#include "gyro.h"
#include <cmath>

#define CENTER_BASE 1500
#define unitsFC 360                          // Units in a full circle
#define dutyScale 1000                       // Scale duty cycle to 1/000ths
#define dcMin 29                             // Minimum duty cycle
#define dcMax 971                            // Maximum duty cycle
#define q2min unitsFC/4                      // For checking if in 1st uadrant
#define q3max q2min * 3  

Ticker servo_ticker;
Ticker servo_feedback_ticker;
PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);
BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);
BusInOut qti_pin(D4,D5,D6,D7);
DigitalInOut pin8(D8);
DigitalOut LEDR(D3);
DigitalOut LEDL(D2);
DigitalOut LEDP(D1);

using mbed::callback;
using namespace std::literals::chrono_literals;

/**
 * A Clock service that demonstrate the GattServer features.
 *
 * The clock service host three characteristics that model the current hour,
 * minute and second of the clock. The value of the second characteristic is
 * incremented automatically by the system.
 *
 * A client can subscribe to updates of the clock characteristics and get
 * notified when one of the value is changed. Clients can also change value of
 * the second, minute and hour characteristric.
 */
class ClockService : public ble::GattServer::EventHandler {
public:
    ClockService() :
        _x_char("0000b241-0000-1000-8000-00805f9b34fb", 0),
        _y_char("0000b242-0000-1000-8000-00805f9b34fb", 0),
        _fb_char("0000b243-0000-1000-8000-00805f9b34fb", 0),
        _ac_char("0000b244-0000-1000-8000-00805f9b34fb", 0),
        _clock_service(
            /* uuid */ "0000b240-0000-1000-8000-00805f9b34fb",
            /* characteristics */ _clock_characteristics,
            /* numCharacteristics */ sizeof(_clock_characteristics) /
                                     sizeof(_clock_characteristics[0])
        )
    {
        /* update internal pointers (value, descriptors and characteristics array) */
        _clock_characteristics[0] = &_x_char;
        _clock_characteristics[1] = &_y_char;
        _clock_characteristics[2] = &_fb_char;
        _clock_characteristics[3] = &_ac_char;

        /* setup authorization handlers */
        _x_char.setWriteAuthorizationCallback(this, &ClockService::authorize_client_write);
        _y_char.setWriteAuthorizationCallback(this, &ClockService::authorize_client_write);
        _fb_char.setWriteAuthorizationCallback(this, &ClockService::authorize_client_write);
        _ac_char.setWriteAuthorizationCallback(this, &ClockService::authorize_client_write);
    }


    void start(BLE &ble, events::EventQueue &event_queue)
    {
        _server = &ble.gattServer();
        _event_queue = &event_queue;

        printf("Registering demo service\r\n");
        ble_error_t err = _server->addService(_clock_service);

        if (err) {
            printf("Error %u during demo service registration.\r\n", err);
            return;
        }

        /* register handlers */
        _server->setEventHandler(this);

        printf("clock service registered\r\n");
        printf("service handle: %u\r\n", _clock_service.getHandle());
        printf("x characteristic value handle %u\r\n", _x_char.getValueHandle());
        printf("y characteristic value handle %u\r\n", _y_char.getValueHandle());
        printf("FeedBack characteristic value handle %u\r\n", _fb_char.getValueHandle());
        printf("Acceleration characteristic value handle %u\r\n", _ac_char.getValueHandle());

        _event_queue->call_every(1000ms, callback(this, &ClockService::getACFB));
        //_event_queue->call_every(50ms, callback(this, &ClockService::getCalDis));
        _event_queue->call_every(8ms, callback(this, &ClockService::runBBcar));
    }

    /* GattServer::EventHandler */
      private:
          /**
     * Handler called when a notification or an indication has been sent.
     */
    void onDataSent(const GattDataSentCallbackParams &params) override
    {
        printf("sent updates\r\n");
    }

    /**
     * Handler called after an attribute has been written.
     */
    void onDataWritten(const GattWriteCallbackParams &params) override
    {
        printf("data written:\r\n");
        printf("connection handle: %u\r\n", params.connHandle);
        printf("attribute handle: %u", params.handle);
        if (params.handle == _x_char.getValueHandle()) {
            printf(" (X characteristic)\r\n");
        } else if (params.handle == _y_char.getValueHandle()) {
            printf(" (y characteristic)\r\n");
        } else if (params.handle == _fb_char.getValueHandle()) {
            printf(" (FeedBack characteristic)\r\n");
        } else if (params.handle == _ac_char.getValueHandle()) {
            printf(" (Acceleration characteristic)\r\n");
        } else {
            printf("\r\n");
        }
        printf("write operation: %u\r\n", params.writeOp);
        printf("offset: %u\r\n", params.offset);
        printf("length: %u\r\n", params.len);
        printf("data: ");

        for (size_t i = 0; i < params.len; ++i) {
            printf("%02X", params.data[i]);
        }

        printf("\r\n");
          }

          /**
           * Handler called after an attribute has been read.
     */
    void onDataRead(const GattReadCallbackParams &params) override
    {
        printf("data read:\r\n");
        printf("connection handle: %u\r\n", params.connHandle);
        printf("attribute handle: %u", params.handle);
        if (params.handle == _x_char.getValueHandle()) {
            printf(" (x characteristic)\r\n");
        } else if (params.handle == _y_char.getValueHandle()) {
            printf(" (y characteristic)\r\n");
        } else if (params.handle == _fb_char.getValueHandle()) {
            printf(" (FeedBack characteristic)\r\n");
        } else if (params.handle == _ac_char.getValueHandle()) {
            printf(" (Acceleration characteristic)\r\n");
        } else {
            printf("\r\n");
        }
    }

    /**
     * Handler called after a client has subscribed to notification or indication.
     *
     * @param handle Handle of the characteristic value affected by the change.
     */
    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) override
    {
        printf("update enabled on handle %d\r\n", params.attHandle);
    }

    /**
     * Handler called after a client has cancelled his subscription from
     * notification or indication.
     *
     * @param handle Handle of the characteristic value affected by the change.
     */
    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) override
    {
        printf("update disabled on handle %d\r\n", params.attHandle);
    }

    /**
     * Handler called when an indication confirmation has been received.
     *
     * @param handle Handle of the characteristic value that has emitted the
     * indication.
     */
    void onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params) override
    {
        printf("confirmation received on handle %d\r\n", params.attHandle);
    }

    

private:
    /**
     * Handler called when a write request is received.
     *
     * This handler verify that the value submitted by the client is valid before
     * authorizing the operation.
     */
    void authorize_client_write(GattWriteAuthCallbackParams *e)
    {
        printf("characteristic %u write authorization\r\n", e->handle);

        if (e->offset != 0) {
            printf("Error invalid offset\r\n");
            e->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
            return;
        }

        if (e->len != 1) {
            printf("Error invalid len\r\n");
            e->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
            return;
        }

        if ((e->data[0] >= 60) ||
            ((e->data[0] >= 24) && (e->handle == _x_char.getValueHandle()))) {
            printf("Error invalid data\r\n");
            e->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED;
            return;
        }

        e->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
    }


    Accelerometer accelerometer;
    Gyro gyroscope;
    double rawAccelerationData[3];
    double calibratedAccelerationData[3];
    double rawGyroData[3];
    double calibratedGyroData[3];

    int trace_value;
    bool pos = false;
    bool neg = false;
    int finish_cnt;
    int cd_cnt;
    int turn_cnt;
    bool finish = false;
    bool wait_park = false;
    bool obstacle = false;
    bool turn_flag = false;
    double ac_distance = 0;
    int ob_cnt = 0; // Count the time facing obstacles
    bool parking = false;
    int park_cnt = 0;

#define roundFactor 73
#define parkFactor 40


    void runBBcar() {    
        parallax_qti qti1(qti_pin);
        int pattern;
        pattern = (int)qti1;
        parallax_laserping  ping1(pin8);
        //if ((float)ping1 < 40) {
        //    printf("%5f\n", (float)ping1);
        //}
        LEDP = parking;
        if ((float)ping1 < 20) {
            obstacle = true;
            //printf("Obstacle(s) detected!!\n");
        } else {
            obstacle = false;
        }

        if (obstacle == true && turn_flag == false && parking == false && finish == false) {
            ob_cnt += 1;
            turn_cnt = 0;
            if (ob_cnt >= 200)
                turn_flag = true;
            LEDR = 1;
            LEDL = 1;
            car.stop();
        } else if (turn_flag == true) {
            ob_cnt = 0;
            turn_cnt += 1;
            if (turn_cnt >= 35)
                turn_flag = false;
            LEDR = 0;
            car.roundA(200);
        } else if (parking == true) {
            if (park_cnt <= parkFactor) {
                park_cnt += 1;
                car.roundA(130);
            } else if (park_cnt >= parkFactor && park_cnt < 4 * parkFactor) {
                park_cnt += 1;
                car.goStraight(70);
            } else if (park_cnt >= 4 * parkFactor && park_cnt < 5 * parkFactor) {
                park_cnt += 1;
                car.roundB(130);
            } else {
                car.stop();
                LEDR = 1;
                LEDL = 1;
            }
        } else {
            LEDR = 0;
            LEDL = 0;
            if (trace_value == 0b0110) {
                pos = false;
                neg = false;
            } else if (trace_value == 0b0011 || trace_value == 0b0001) {
                pos = false;
                neg = true;
            } else if(trace_value == 0b0011 || trace_value == 0b1000) {
                pos = true;
                neg = false;
            } else {
                pos = false;
                neg = false;
            }

            if (finish == false) {
                if (pattern == 0b0000) {
                    if (pos == true) {
                        car.turn(70, 0.2);
                    } else if (neg == true) {
                        car.turn(70, -0.2);
                    } else {
                        car.goStraight(70);
                    }
                } else {
                    cd_cnt += 1;
                    trace_value = pattern;
                    switch (pattern) {
                        case 0b1000: car.turn(90, 0.01); LEDL = 1; LEDR = 0; break;
                        case 0b1100: car.turn(85, 0.05); LEDL = 1; LEDR = 0;  break;
                        case 0b1110: car.turn(80, 0.05);  LEDL = 1; LEDR = 0;  break; // Branch
                        case 0b0100: car.turn(70, 0.4);  LEDL = 1; LEDR = 0;  break;
                        case 0b0110: car.goStraight(70); break;
                        case 0b0010: car.turn(70, -0.4); LEDR = 1; LEDL = 0; break;
                        case 0b0111: car.turn(80, -0.05); LEDR = 1; LEDL = 0; break; // Branch
                        case 0b0011: car.turn(85, -0.05);LEDR = 1; LEDL = 0; break;
                        case 0b0001: car.turn(90, -0.01);LEDR = 1; LEDL = 0; break;
                        // If branch + obstacle => turn left
                        case 0b1111: 
                            if (ping1 < 50) {
                                wait_park = true;
                                car.turn(70, 0.2); 
                            } else {
                                if (wait_park == true)
                                    parking = true;
                            }
                            break; 
                        // Special task which have CD times for 2 second
                        case 0b1001: 
                            if (ping1 < 50) {
                                car.turn(70, 0.2); 
                            } else {
                                if(cd_cnt > 200) finish = true;
                            }
                             break;
                    }
                    trace_value = pattern;
                }    
            } else {
                cd_cnt = 0;
                if (finish_cnt < roundFactor) {
                    car.roundA(200);
                    finish_cnt += 1;
                } else if (finish_cnt >= roundFactor && finish_cnt < 2.35 * roundFactor) {
                    car.roundB(200);
                    finish_cnt += 1;
                } else {
                    finish = false;
                    finish_cnt = 0;
                }
            }
        }
    }

    
    double calculateDistance(double xAcceleration, double yAcceleration) {
        double totalDistance = 0.0;
        double xVelocity = 0.0;
        double yVelocity = 0.0;
        double xPosition = 0.0;
        double yPosition = 0.0;

        // Update velocities using acceleration
        xVelocity += xAcceleration;
        yVelocity += yAcceleration;
        // Update positions using velocity
        xPosition += xVelocity;
        yPosition += yVelocity;
        // Calculate distance using Pythagorean theorem
        double distance = sqrt(xPosition * xPosition + yPosition * yPosition);
        // Add to total distance
        totalDistance += distance;
        
        return totalDistance;
    }   

    double getCalDis() {
        double cal_distance = 0;
        accelerometer.GetAcceleromterSensor(rawAccelerationData);
        // Calibrate accelerometer data
        accelerometer.GetAcceleromterCalibratedData(calibratedAccelerationData);
        // Read raw gyroscope data
        gyroscope.GetGyroSensor(rawGyroData);
        // Calibrate gyroscope data
        gyroscope.GetGyroCalibratedData(calibratedGyroData);

        /////////////////////////////////////////////////////////////////////
        cal_distance = calculateDistance(calibratedAccelerationData[0], calibratedAccelerationData[1]);
        
        return cal_distance;
        /////////////////////////////////////////////////////////////////////
    }

    void getACFB()
    {
        uint8_t x, y, fb, ac;
        
/*
        accelerometer.GetAcceleromterSensor(rawAccelerationData);
        // Calibrate accelerometer data
        accelerometer.GetAcceleromterCalibratedData(calibratedAccelerationData);
        // Read raw gyroscope data
        gyroscope.GetGyroSensor(rawGyroData);
        // Calibrate gyroscope data
        gyroscope.GetGyroCalibratedData(calibratedGyroData);
*/

        ble_error_t err = _x_char.get(*_server, x);
        if (err) {
            printf("read of the x value returned error %u\r\n", err);
            return;
        }

        ble_error_t err1 = _y_char.get(*_server, y);
        if (err1) {
            printf("read of the y value returned error %u\r\n", err1);
            return;
        }

        ble_error_t err2 = _fb_char.get(*_server, fb);
        if (err2) {
            printf("read of the fb value returned error %u\r\n", err2);
            return;
        }

        ble_error_t err3 = _ac_char.get(*_server, ac);
        if (err3) {
            printf("read of the ac value returned error %u\r\n", err3);
            return;
        }
      
        x = calibratedAccelerationData[0]*100;
        y = calibratedAccelerationData[1]*100;
        fb = (car.servo0.angle < 0)? 0 : (car.servo0.angle)*6.5*3.14/360;

        double current_dis = getCalDis();
        ac_distance += current_dis * 100;
        ac = int(ac_distance);
        
        printf("cal dist = %f\n", ac_distance);
        printf("distance = %f\n", (car.servo0.angle)*6.5*3.14/360);


        err = _x_char.set(*_server, x);
        if (err) {
            printf("write of the x value returned error %u\r\n", err);
            return;
        }
        err1 = _y_char.set(*_server, y);
        if (err1) {
            printf("write of the y value returned error %u\r\n", err1);
            return;
        }
        err2 = _fb_char.set(*_server, fb);
        if (err2) {
            printf("write of the fb value returned error %u\r\n", err2);
            return;
        }
        
        err3 = _ac_char.set(*_server, ac);
        if (err3) {
            printf("write of the ac value returned error %u\r\n", err3);
            return;
        }
        
    }



private:
    /**
     * Read, Write, Notify, Indicate  Characteristic declaration helper.
     *
     * @tparam T type of data held by the characteristic.
     */
    template<typename T>
    class ReadWriteNotifyIndicateCharacteristic : public GattCharacteristic {
    public:
        /**
         * Construct a characteristic that can be read or written and emit
         * notification or indication.
         *
         * @param[in] uuid The UUID of the characteristic.
         * @param[in] initial_value Initial value contained by the characteristic.
         */
        ReadWriteNotifyIndicateCharacteristic(const UUID & uuid, const T& initial_value) :
            GattCharacteristic(
                /* UUID */ uuid,
                /* Initial value */ &_value,
                /* Value size */ sizeof(_value),
                /* Value capacity */ sizeof(_value),
                /* Properties */ GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY |
                                 GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE,
                /* Descriptors */ nullptr,
                /* Num descriptors */ 0,
                /* variable len */ false
            ),
            _value(initial_value) {
        }

        /**
         * Get the value of this characteristic.
         *
         * @param[in] server GattServer instance that contain the characteristic
         * value.
         * @param[in] dst Variable that will receive the characteristic value.
         *
         * @return BLE_ERROR_NONE in case of success or an appropriate error code.
         */
        ble_error_t get(GattServer &server, T& dst) const
        {
            uint16_t value_length = sizeof(dst);
            return server.read(getValueHandle(), &dst, &value_length);
        }

        /**
         * Assign a new value to this characteristic.
         *
         * @param[in] server GattServer instance that will receive the new value.
         * @param[in] value The new value to set.
         * @param[in] local_only Flag that determine if the change should be kept
         * locally or forwarded to subscribed clients.
         */
        ble_error_t set(GattServer &server, const uint8_t &value, bool local_only = false) const
        {
            return server.write(getValueHandle(), &value, sizeof(value), local_only);
        }

    private:
        uint8_t _value;
    };

    private:
    GattServer *_server = nullptr;
    events::EventQueue *_event_queue = nullptr;

    GattService _clock_service;
    GattCharacteristic* _clock_characteristics[4];

    ReadWriteNotifyIndicateCharacteristic<uint8_t> _x_char;
    ReadWriteNotifyIndicateCharacteristic<uint8_t> _y_char;
    ReadWriteNotifyIndicateCharacteristic<uint8_t> _fb_char;
    ReadWriteNotifyIndicateCharacteristic<uint8_t> _ac_char;
};

int main() {
    mbed_trace_init();
////////////////////////////////////////////////////////////////////////////////////
    BLE &ble = BLE::Instance();
    events::EventQueue event_queue;
    ClockService demo_service;
    /* this process will handle basic ble setup and advertising for us */
    GattServerProcess ble_process(event_queue, ble);
    /* once it's done it will let us continue with our demo */
    ble_process.on_init(callback(&demo_service, &ClockService::start));
    ble_process.start();
////////////////////////////////////////////////////////////////////////////////////

}