# Test Documentation
In this Readme we figure out how to move the car and how the pins are actually mapped

## 1. Unexpected Hardware Mapping

Through testing, we confirmed that the physical pins connected to the S1, S2, S3, and S4 switches on the daughterboard are directly wired to the **L298N Motor Driver direction inputs** (IN1, IN2, IN3, IN4).

* **Location:** All four direction control pins are mapped to **Port 2 (P2)**, specifically **P2.0 through P2.3**.

* **Logic:** The system uses **Active Low** logic for direction control. Setting a pin LOW (``0``) activates that direction; setting it HIGH (``1``) deactivates it.

* **Enable Pins:** The motor driver's speed/enable pins (ENA/ENB) appear to be **permanently enabled** by default on the hardware, meaning setting the direction pins is enough to start motor movement.

### Pin-to-Action Mapping

The final confirmed physical mapping, used for the direction functions, is as follows:

| Physical Button Pin (P2.x) | Direction Pin | Motor Side | Action (When set LOW) | 
 | ----- | ----- | ----- | ----- | 
| **P2.0 (S1)** | INx | Right | **Forward** | 
| **P2.1 (S2)** | INx | Right | **Backward** | 
| **P2.2 (S3)** | INx | Left | **Backward** | 
| **P2.3 (S4)** | INx | Left | **Forward** | 

## 2. Direction Control Logic

Since the microcontroller cannot read the buttons, the code was refactored to **OUTPUT** the required LOW/HIGH combinations directly onto Port 2, simulating the combination of button presses necessary to achieve movement.

| Desired Movement | Right Motor Direction | Left Motor Direction | P2.0 (R-FWD) | P2.1 (R-BACK) | P2.2 (L-BACK) | P2.3 (L-FWD) | 
 | ----- | ----- | ----- | ----- | ----- | ----- | ----- | 
| **Forward** | Forward | Forward | 0 (Active) | 1 (Inactive) | 1 (Inactive) | 0 (Active) | 
| **Backward** | Backward | Backward | 1 (Inactive) | 0 (Active) | 0 (Active) | 1 (Inactive) | 
| **Spin Left** | Forward | Backward | 0 (Active) | 1 (Inactive) | 0 (Active) | 1 (Inactive) | 
| **Spin Right** | Backward | Forward | 1 (Inactive) | 0 (Active) | 1 (Inactive) | 0 (Active) | 

## 3. Turning Refinement

To achieve dramatic and precise turning, two key refinements were implemented:

1. **Differential Steering (Spin Turn):** The ``move_left()`` and ``move_right()`` functions were adapted to use **spin turns**, where the wheels on one side rotate forward while the wheels on the opposite side rotate backward. This allows the robot to pivot in place for a sharp rotation. 

2. **Time Control for Angle:** Since PWM (speed control) is unavailable, the degree of rotation is controlled solely by the **duration (``delay_ms``)** the spin command is active. An initial duration of **250ms** was found to provide a reasonably sharp, close-to-90-degree turn for this robot chassis.