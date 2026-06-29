#include <Servo.h>

Servo solarServo;

// LDR Pins
const int ldrLeft = A0;
const int ldrRight = A1;

// Servo Pin
const int servoPin = 9;

// Servo Parameters
int servoAngle = 90;
const int minAngle = 15;
const int maxAngle = 165;

// Threshold for movement
const int threshold = 30;

// Number of samples for moving average
const int samples = 10;

// Function to calculate moving average
int readAverage(int pin)
{
  long sum = 0;

  for (int i = 0; i < samples; i++)
  {
    sum += analogRead(pin);
    delay(2);
  }

  return sum / samples;
}

void setup()
{
  Serial.begin(9600);

  solarServo.attach(servoPin);

  solarServo.write(servoAngle);

  delay(500);
}

void loop()
{
  // Read filtered sensor values
  int leftValue = readAverage(ldrLeft);
  int rightValue = readAverage(ldrRight);

  // Calculate difference
  int difference = leftValue - rightValue;

  // Print values to Serial Monitor
  Serial.print("Left: ");
  Serial.print(leftValue);

  Serial.print("  Right: ");
  Serial.print(rightValue);

  Serial.print("  Difference: ");
  Serial.println(difference);

  // Rotate only if threshold exceeded
  if (abs(difference) > threshold)
  {
    if (difference > 0)
    {
      servoAngle++;
    }
    else
    {
      servoAngle--;
    }

    // Limit servo movement
    servoAngle = constrain(servoAngle, minAngle, maxAngle);

    // Move servo
    solarServo.write(servoAngle);
  }

  // Wait before next reading (energy-efficient tracking)
  delay(100);
}
