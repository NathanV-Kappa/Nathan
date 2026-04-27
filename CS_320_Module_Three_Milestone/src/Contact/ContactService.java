/**
 * Nathan Vanderpool
 * CS-320
 * March 22, 2026
 */

package Contact;

import java.util.HashMap;
import java.util.Map;


/**
 * In-memory service for managing contacts.
 * Provides operations to add contacts (with unique ID enforcement), delete by ID,
 * and update the allowed fields (firstName, lastName, phone, address).
 * All operations throw IllegalArgumentException on invalid input or missing contacts.
 */
public class ContactService {
    private final Map<String, Contact> contacts = new HashMap<>();

    
    /**
     * Adds a contact if its ID is not already in use.
     *
     * @param contact the contact to add
     * @throws IllegalArgumentException if contact is null or ID already exists
     */
    public void addContact(Contact contact) {
        if (contact == null) {
            throw new IllegalArgumentException("Contact cannot be null");
        }
        
        String id = contact.getContactID();
        if (contacts.containsKey(id)) {
            throw new IllegalArgumentException("Contact ID already exists: " + id);
        }
        contacts.put(id, contact);
    }

    
    /**
     * Deletes the contact with the specified ID.
     *
     * @param contactID the ID of the contact to delete
     * @throws IllegalArgumentException if ID is null/empty or not found
     */
    public void deleteContact(String contactID) {
        if (contactID == null || contactID.isEmpty()) {
            throw new IllegalArgumentException("Contact ID cannot be null or empty");
        }
        
        if (!contacts.containsKey(contactID)) {
            throw new IllegalArgumentException("Contact ID not found: " + contactID);
        }
        contacts.remove(contactID);
    }

    
    /**
     * Updates the first name of the contact with the given ID.
     *
     * @param contactID the ID of the contact to update
     * @param firstName the new first name
     * @throws IllegalArgumentException if contact not found or value invalid
     */
    public void updateFirstName(String contactID, String firstName) {
        Contact contact = getContactOrThrow(contactID);
        contact.setFirstName(firstName);
    }

    
    /**
     * Updates the last name of the contact with the given ID.
     *
     * @param contactID the ID of the contact to update
     * @param lastName  the new last name
     * @throws IllegalArgumentException if contact not found or value invalid
     */
    public void updateLastName(String contactID, String lastName) {
        Contact contact = getContactOrThrow(contactID);
        contact.setLastName(lastName);
    }

    
    /**
     * Updates the phone number of the contact with the given ID.
     *
     * @param contactID the ID of the contact to update
     * @param phone     the new phone number
     * @throws IllegalArgumentException if contact not found or value invalid
     */
    public void updatePhone(String contactID, String phone) {
        Contact contact = getContactOrThrow(contactID);
        contact.setPhone(phone);
    }

    
    /**
     * Updates the address of the contact with the given ID.
     *
     * @param contactID the ID of the contact to update
     * @param address   the new address
     * @throws IllegalArgumentException if contact not found or value invalid
     */
    public void updateAddress(String contactID, String address) {
        Contact contact = getContactOrThrow(contactID);
        contact.setAddress(address);
    }

    // Helper method
    private Contact getContactOrThrow(String contactID) {
        if (contactID == null || contactID.isEmpty()) {
            throw new IllegalArgumentException("Contact ID cannot be null or empty");
        }
        
        Contact contact = contacts.get(contactID);
        if (contact == null) {
            throw new IllegalArgumentException("Contact ID not found: " + contactID);
        }
        return contact;
    }

    
    /**
     * Retrieves a contact by ID (for testing purposes only).
     *
     * @param contactID the ID to look up
     * @return the contact, or null if not found
     */
    public Contact getContact(String contactID) {
        return contacts.get(contactID);
    }
}
