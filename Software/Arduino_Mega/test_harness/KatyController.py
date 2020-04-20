import time

from pymata4.pymata4 import Pymata4


class KatyController:
    def __init__(self) -> None:
        self.arduino = Pymata4(com_port='COM3', baud_rate=57600, arduino_wait=1)
        # Mappings are from Motorola MC68008 to Arduino pins
        self.address_bus_pins = [22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 53, 51, 49, 47]
        self.address_bus_as_inputs()

        self.data_bus_pins = [31, 33, 35, 37, 39, 41, 43, 45]
        self.data_bus_as_inputs()

        # DTACK pulse is active low, so we set it to high initially
        self.dtack_control_pin = 27
        self.arduino.set_pin_mode_digital_output(self.dtack_control_pin)
        self.arduino.digital_pin_write(self.dtack_control_pin, 1)

        self.reset_pin = 29
        # self.halt_pin = 29

        self.arduino.set_pin_mode_digital_output(self.reset_pin)
        # self.arduino.set_pin_mode_digital_input(self.halt_pin)
        # Set reset circuit to default - active state
        self.arduino.digital_pin_write(self.reset_pin, 1)

        # Reset the circuit: the arduino was booted in high-impedance mode so there's no telling in what state the
        # circuit is.
        self.reset_chip()

    def reset_chip(self) -> None:
        self.arduino.digital_pin_write(self.reset_pin, 0)
        time.sleep(0.1)
        self.arduino.digital_pin_write(self.reset_pin, 1)
        # Wait for circuit reset sequence to complete
        time.sleep(1)

    def dtack_pulse(self) -> None:
        """
        Sends an DTACK active (low) pulse to the Motorola chip to end one bus cycle
        :return: None
        """
        self.arduino.digital_pin_write(self.dtack_control_pin, 0)
        # time.sleep(0.05)
        self.arduino.digital_pin_write(self.dtack_control_pin, 1)
        # The Arduino or circuit needs time to recover
        time.sleep(0.15)

    # --- Address bus operations ---
    def address_bus_as_inputs(self) -> None:
        for address_bus_pin in self.address_bus_pins:
            self.arduino.set_pin_mode_digital_input(address_bus_pin)

    def data_bus_as_inputs(self) -> None:
        for data_bus_pin in self.data_bus_pins:
            self.arduino.set_pin_mode_digital_input(data_bus_pin)

    def data_bus_as_outputs(self) -> None:
        for data_bus_pin in self.data_bus_pins:
            self.arduino.set_pin_mode_digital_output(data_bus_pin)

    def data_bus_write(self, value) -> None:
        assert value < 256, f'The 68008 has a data bus of 8 bits, so it contain no value larger than 256: got {value}'
        self.data_bus_as_outputs()

        # Convert value to binary string that we can iterate over
        binary_value = "{0:08b}".format(value)

        # Loop over the data bus pins and write the corresponding value from the binary string to the pin
        for val_index, data_bus_pin in enumerate(self.data_bus_pins):
            bit = int(binary_value[val_index])
            self.arduino.digital_pin_write(data_bus_pin, bit)

    def address_bus_read(self) -> int:
        address: int = 0

        for address_pin, arduino_pin in enumerate(self.address_bus_pins):
            pin_state = self.arduino.digital_pins[arduino_pin].current_value
            address = address + pin_state * 2 ** address_pin

        return address

    def data_bus_read(self) -> int:
        data: int = 0

        for data_pin, arduino_pin in enumerate(self.data_bus_pins):
            pin_state = self.arduino.digital_pins[arduino_pin].current_value
            data = data + pin_state * 2 ** data_pin

        return data

    def shutdown(self) -> None:
        self.arduino.shutdown()
