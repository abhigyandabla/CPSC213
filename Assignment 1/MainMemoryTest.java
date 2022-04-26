package arch.sm213.machine.student;

import org.junit.Test;
import machine.AbstractMainMemory;
import static org.junit.jupiter.api.Assertions.fail;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class MainMemoryTest {

    MainMemory mainMemory = new MainMemory(8);

    @Test
    public void testIsAccessAligned() {
        // This test is testing the isAccessAligned() method with various examples
        assertFalse(mainMemory.isAccessAligned(1, 4));
        assertTrue(mainMemory.isAccessAligned(0, 8));
        assertFalse(mainMemory.isAccessAligned(1, 8));
        assertFalse(mainMemory.isAccessAligned(1, 2));
        assertTrue(mainMemory.isAccessAligned(0, 4));
        assertTrue(mainMemory.isAccessAligned(2,2));
        assertTrue(mainMemory.isAccessAligned(4,4 ));
    }

    @Test
    public void testBytesToIntegerRandomNumber(){
        // Using random integer with variety of numbers and letters and seeing if it turns
        // to correct decimal integer
        byte zero = (byte)0x23;
        byte one = (byte)0xae;
        byte two = (byte)0x61;
        byte three = (byte)0x02;
        assertEquals(598630658, mainMemory.bytesToInteger(zero, one, two, three));
    }

    @Test
    public void testBytesToIntegerZero(){
        // Testing if 00 00 00 00 (base 16) is converted successfully to 0 (base 10)
        byte zero = (byte)0x00;
        byte one = (byte)0x00;
        byte two = (byte)0x00;
        byte three = (byte)0x00;
        assertEquals(0, mainMemory.bytesToInteger(zero, one, two, three));
    }

    @Test
    public void testBytesToIntegerMinusOne(){
        // Testing if ff ff ff ff (base 16) is converted successfully to -1 (base 10)
        // This also tests negative values
        byte zero = (byte)0xff;
        byte one = (byte)0xff;
        byte two = (byte)0xff;
        byte three = (byte)0xff;
        assertEquals(-1, mainMemory.bytesToInteger(zero, one, two, three));
    }

    @Test
    public void testIntegerToBytes() {
        // This test is turning decimal integers to 4-byte hex numbers

        byte[] byteTwo = mainMemory.integerToBytes(-69420); // Trying negative number,
                                                              // should be FF FE F0 D4
        assertEquals((byte)0xff, byteTwo[0]);
        assertEquals((byte)0xfe, byteTwo[1]);
        assertEquals((byte)0xf0, byteTwo[2]);
        assertEquals((byte)0xd4, byteTwo[3]);

        byte[] byteThree = mainMemory.integerToBytes(12345678); // Trying large number,
                                                                  // should be 00 BC 61 4E
        assertEquals((byte)0x00, byteThree[0]);
        assertEquals((byte)0xbc, byteThree[1]);
        assertEquals((byte)0x61, byteThree[2]);
        assertEquals((byte)0x4e, byteThree[3]);

        byte[] byteFour = mainMemory.integerToBytes(69); // Completely random number which is 45 in hex
        assertEquals((byte)0x00, byteFour[0]);
        assertEquals((byte)0x00, byteFour[1]);
        assertEquals((byte)0x00, byteFour[2]);
        assertEquals((byte)0x45, byteFour[3]);

        byte[] byteFive = mainMemory.integerToBytes(0); // Trying 0
        assertEquals((byte)0x00, byteFive[0]);
    }

    @Test
    public void testSetAndGetMethodFourBytes(){
        // Seeing if setter works with a regular 4-byte integer
        // Also testing the getter in this method by using it to retrieve data
        byte[] testing = {(byte)0x23,
                          (byte)0xae,
                          (byte)0x61,
                          (byte)0x02};
        try {
            mainMemory.set(0, testing);

            byte[] retrieved = mainMemory.get(0, 4);

            assertEquals(testing[0], retrieved[0]);
            assertEquals(testing[1], retrieved[1]);
            assertEquals(testing[2], retrieved[2]);
            assertEquals(testing[3], retrieved[3]);
        } catch(AbstractMainMemory.InvalidAddressException e) {
            fail("Test should pass.");
        }
    }

    @Test
    public void testSetAndGetMethodTwoBytes(){
        // Same as last example but trying two bytes and a different address (still aligned)
        byte[] testing = {(byte)0x23,
                          (byte)0xae};
        try {
            mainMemory.set(2, testing);

            byte[] retrieved = mainMemory.get(2, 2);

            assertEquals(testing[0], retrieved[0]);
            assertEquals(testing[1], retrieved[1]);
        } catch(AbstractMainMemory.InvalidAddressException e) {
            fail("Test should pass.");
        }
    }

    @Test
    public void testSetAndGetMethodLengthLessThanZero(){
        // Length in getter less than 0 - should fail
        byte[] testing = {(byte)0x23,
                (byte)0xae};
        try {
            mainMemory.set(2, testing);
            mainMemory.get(2, -2);
            fail("Test should fail");
        } catch(AbstractMainMemory.InvalidAddressException e) {
            // Should catch
        }
    }

    @Test
    public void testSetMethodOutOfMemory(){
        // Setting out of bounds of memory length - should fail
        byte[] testing = {(byte)0x23,
                (byte)0xae};
        try {
            mainMemory.set(8, testing);
            fail("Test should fail");
        } catch(AbstractMainMemory.InvalidAddressException e) {
            // Should catch
        }
    }

    @Test
    public void testGetMethodOutOfMemory(){
        // Getting out of bounds of memory - should fail
        try {
            mainMemory.get(8, 4);
            fail("Test should fail");
        } catch(AbstractMainMemory.InvalidAddressException e) {
            // Should catch
        }
    }

    @Test
    public void testSetMethodAddressLessThanZero(){
        // Setting address less than 0 - should fail
        byte[] testing = {(byte)0x23,
                (byte)0xae,
                (byte)0x61,
                (byte)0x02};
        try {
            mainMemory.set(-2, testing);
            fail("Exception should be thrown");
        } catch(AbstractMainMemory.InvalidAddressException e) {
            // Should catch
        }
    }

    @Test
    public void testGetMethodAddressLessThanZero(){
        // Getting address less than 0 - should fail
        try {
            mainMemory.get(-2, 2);
            fail("Test should fail");
        } catch(AbstractMainMemory.InvalidAddressException e) {
            // Should catch
        }
    }


}
