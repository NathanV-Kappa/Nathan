/**
 * Nathan Vanderpool
 * CS-320
 * March 22, 2026
 */

package Tests;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import Contact.Contact;
import Contact.ContactService;

class ContactServiceTest {

    private ContactService service;

    
    @BeforeEach
    void setUp() {
        service = new ContactService();
    }

    
    @Test
    void testAddContact() {
        Contact contact = new Contact("1234567890", "John", "Doe", "1234567890", "123 Main St");
        service.addContact(contact);
        assertNotNull(service.getContact("1234567890"));
    }

    
    @Test
    void testAddDuplicateID() {
        Contact c1 = new Contact("123", "John", "Doe", "1234567890", "123 Main St");
        Contact c2 = new Contact("123", "Jane", "Doe", "9876543210", "456 Elm St");
        service.addContact(c1);
        assertThrows(IllegalArgumentException.class, () -> service.addContact(c2));
    }

    
    @Test
    void testDeleteContact() {
        Contact contact = new Contact("123", "John", "Doe", "1234567890", "123 Main St");
        service.addContact(contact);
        service.deleteContact("123");
        assertNull(service.getContact("123"));
    }

    
    @Test
    void testDeleteNonExistent() {
        assertThrows(IllegalArgumentException.class, () -> service.deleteContact("999"));
    }

    
    @Test
    void testUpdateFields() {
        Contact contact = new Contact("123", "John", "Doe", "1234567890", "123 Main St");
        service.addContact(contact);

        service.updateFirstName("123", "Jane");
        service.updateLastName("123", "Smith");
        service.updatePhone("123", "9876543210");
        service.updateAddress("123", "456 Elm St");

        Contact updated = service.getContact("123");
        assertEquals("Jane", updated.getFirstName());
        assertEquals("Smith", updated.getLastName());
        assertEquals("9876543210", updated.getPhone());
        assertEquals("456 Elm St", updated.getAddress());
    }

    
    @Test
    void testUpdateNonExistentContact() {
        assertThrows(IllegalArgumentException.class, () ->
                service.updateFirstName("999", "Jane"));
    }
}
