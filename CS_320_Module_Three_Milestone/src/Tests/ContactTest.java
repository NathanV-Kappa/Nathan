/**
 * Nathan Vanderpool
 * CS-320
 * March 22, 2026
 */

package Tests;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import Contact.Contact;

class ContactTest {

	
    @Test
    void testValidContactCreation() {
        Contact contact = new Contact("1234567890", "John", "Doe", "1234567890", "123 Main St");
        assertEquals("1234567890", contact.getContactID());
        assertEquals("John", contact.getFirstName());
        assertEquals("Doe", contact.getLastName());
        assertEquals("1234567890", contact.getPhone());
        assertEquals("123 Main St", contact.getAddress());
    }

    
    @Test
    void testContactIDTooLong() {
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("12345678901", "John", "Doe", "1234567890", "123 Main St"));
    }

    
    @Test
    void testContactIDNull() {
        assertThrows(IllegalArgumentException.class, () ->
                new Contact(null, "John", "Doe", "1234567890", "123 Main St"));
    }

    
    @Test
    void testFirstNameTooLong() {
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("123", "VeryLongName", "Doe", "1234567890", "123 Main St"));
    }

    
    @Test
    void testLastNameTooLong() {
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("123", "John", "VeryLongLastName", "1234567890", "123 Main St"));
    }

    
    @Test
    void testPhoneNotExactly10Digits() {
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("123", "John", "Doe", "123456789", "123 Main St"));
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("123", "John", "Doe", "123-456-7890", "123 Main St"));
    }

    
    @Test
    void testAddressTooLong() {
        String longAddr = "This address is way too long to be accepted by the system now";
        assertThrows(IllegalArgumentException.class, () ->
                new Contact("123", "John", "Doe", "1234567890", longAddr));
    }

    
    @Test
    void testUpdateFields() {
        Contact contact = new Contact("123", "John", "Doe", "1234567890", "123 Main St");
        contact.setFirstName("Jane");
        contact.setLastName("Smith");
        contact.setPhone("9876543210");
        contact.setAddress("456 Elm St");

        assertEquals("Jane", contact.getFirstName());
        assertEquals("Smith", contact.getLastName());
        assertEquals("9876543210", contact.getPhone());
        assertEquals("456 Elm St", contact.getAddress());
    }
}
