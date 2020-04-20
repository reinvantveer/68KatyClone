import unittest

from tqdm import tqdm

from KatyController import KatyController


class TestHarness(unittest.TestCase):
    @classmethod
    def setUpClass(self) -> None:
        self.controller = KatyController()

    @classmethod
    def tearDownClass(self) -> None:
        self.controller.shutdown()

    def test_working_freerunning(self):
        address_space = range(2 ** 20)
        zero = 0x0000

        # For the first 8 cycles, the CPU will set some kind of reset vector and jump there.
        for _ in range(8):
            self.controller.data_bus_write(zero)
            self.controller.dtack_pulse()

        for expected_address in tqdm(1000):
            address = self.controller.address_bus_read()
            self.assertEqual(address, expected_address)
            self.controller.data_bus_write(zero)
            self.controller.dtack_pulse()
