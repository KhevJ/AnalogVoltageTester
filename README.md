# AnalogVoltageTester

### i. Overview

We have created an **analog voltage tester** that compares an input voltage to a threshold. When the value is below that threshold, the system informs the user by indicating that the input voltage is too low.

---

### ii. Operation States

1. **Initial State**  
   - The analog voltage tester accepts any analog voltage input.
   - It identifies the input as a "good voltage" and prints it to the screen.

2. **Threshold Comparison Mode (Button 1 Pressed)**  
   - When the first button is pressed, the tester enters a state where:
     - All input voltages are compared to a predefined threshold.
     - If the input voltage is **below** the threshold:
       - An **LED turns on**, indicating the user should use a higher input voltage.
       - After a few seconds, the LED turns **off**, demonstrating a **circuit reset**.

3. **Return to Initial State (Button 2 Pressed)**  
   - Pressing the second button returns the tester to the initial state.
   - It resumes accepting any input voltage without comparison.

---

### iii. Modules, Drivers, and Peripherals Used

| Module / Peripheral | Purpose / Reason |
|----------------------|------------------|
| **Clock Switching**  | To save power during low-power phases. |
| **UART**             | To display the input voltage and show the status ("good" or "too low"). |
| **CN Interrupt**     | To detect if either of the two buttons is pressed. |
| **ADC**              | To read varying analog input voltages. |
| **Timer**            | To delay screen updates for better readability. |
| **Watchdog Timer**   | To control the LED on-time duration and help with power management. |

---

### iv. Power Saving Measures

- Used the **Watchdog Timer** while in **idle mode** to manage LED timing and reduce energy usage.
- Employed **clock switching** during low-activity phases to further **conserve power**.
