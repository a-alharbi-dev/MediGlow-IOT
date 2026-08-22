# MediGlow – Smart IoT Medication Reminder System

##  Project Overview

**MediGlow** is a smart IoT-based medication reminder system designed to help elderly people and adults manage their daily medications and vitamins more safely and efficiently.

The system provides **visual and audio reminders** when it is time to take medication. It also monitors medication availability and expiration status, helping users avoid missed doses, expired medication, and running out of medicine without notice.

Medication-related data can also be monitored through cloud services, providing better tracking and awareness for caregivers and family members.

---

##  Problem Statement

Managing medication schedules can be challenging, especially for elderly individuals who may experience memory difficulties, have complex medication routines, or have limited experience with technology.

Common problems include:

* Forgetting to take medication on time
* Taking medication after its expiration date
* Running out of medication without noticing
* Difficulty using smartphone-based medication applications

MediGlow addresses these challenges by providing a **simple and elderly-friendly solution that does not depend on complicated screens or constant smartphone interaction**.

---

##  Project Objectives

The main objectives of MediGlow are to:

* Improve medication adherence through visual and audio reminders.
* Provide an easy-to-use solution for elderly users.
* Reduce medication errors such as missed or late doses.
* Monitor medication availability and expiration status.
* Support caregivers and family members through cloud-based monitoring.
* Encourage independent medication management.
* Demonstrate the use of IoT technology in healthcare.
* Provide an affordable solution using readily available hardware components.

---

##  Hardware Components

The project uses the following hardware components:

* **ESP32 Microcontroller**
* **Ultrasonic Sensor**
* **RGB LED**
* **Buzzer**
* **Resistor**
* **Pin Headers**
* **Breadboard**
* **Jumper Wires**

---

##  Tools & Technologies

### Software & Development Tools

* **Arduino IDE** – Used for programming and uploading the ESP32 code.
* **C/C++** – Used to program the ESP32 and control the connected hardware components.
* **ThingSpeak** – Used for cloud-based monitoring and visualization of medication-related data.

### IoT Technologies

* **ESP32 Wi-Fi Connectivity**
* **Cloud Data Monitoring**
* **Sensor-Based Monitoring**
* **Real-Time Alerts**
* **Embedded Systems**

---

##  How It Works

MediGlow monitors the medication container and provides different notifications based on the medication status.

### 1. Normal Status

When the medication is available and within its valid period, the system remains in its normal state.

### 2. Medication Reminder

When the scheduled medication time is reached, the system provides a notification reminding the user to take the medicine.

### 3. Low Medication Level

The ultrasonic sensor is used to monitor the medication level. When the medication is nearly finished, the system indicates that the medication needs to be replenished.

### 4. Expired Medication

The system can indicate when medication has expired, helping users avoid consuming expired medicine.

These different states were demonstrated and tested as part of the project.

---

##  System Components

The MediGlow system combines:

**Sensors → ESP32 → Notifications → Cloud Monitoring**

The **ESP32** acts as the main microcontroller, receiving information from the sensor and controlling the notification components such as the RGB LED and buzzer.

---

##  Cloud Monitoring

Medication information can be stored and monitored through cloud services. This allows caregivers and family members to have better awareness of medication status and helps support safer medication management.

---

##  Testing

The project was tested under different medication conditions, including:

* Medication schedule / reminder time
* Normal medication status
* Medication reminder notification
* Nearly finished medication
* Expired medication

---

##  Key Features

* Medication time reminders
* Audio alerts using a buzzer
*  Visual status indicators using an RGB LED
*  Medication-level monitoring using an ultrasonic sensor
*  Expiration status indication
*  Cloud-based medication monitoring
*  Elderly-friendly design
*  Cost-effective IoT implementation

---

##  Academic Project

**Course:** Internet of Things (IoT) – CCSW-432
**Department:** Software Engineering
**Project:** MediGlow – Smart IoT Medication Reminder System

### Team Members

* Ohood Khaled ALMabdi
* Abrar Obaid ALharbi
* Raghad Ali ALghamdi
* Juwairiyah Arif Shaikh

**Instructor:** Dr. Nadia

---


##  Future Improvements

Possible future improvements include:

* Mobile application integration
* More advanced medication scheduling
* Automatic medication inventory tracking
* Notifications for caregivers
* Improved cloud dashboard
* Multiple medication compartments
* Real-time monitoring and reporting

