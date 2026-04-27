/**
 * Nathan Vanderpool
 * CS-320
 * March 22, 2026
 */

package Contact;

/**
 * Represents a contact entity with required fields and strict validation rules.
 * The contact ID is immutable and unique within the service. All fields are validated
 * on construction and during updates; violations throw IllegalArgumentException.
 */
public class Contact {
    private final String contactID;     // unique, <=10 chars, not null, immutable
    private String firstName;           // required, <=10 chars, not null
    private String lastName;            // required, <=10 chars, not null
    private String phone;               // required, exactly 10 digits, not null
    private String address;             // required, <=30 chars, not null

    
    /**
     * Constructs a new Contact with the specified values.
     * All fields are validated immediately; invalid input throws IllegalArgumentException.
     *
     * @param contactID   the unique identifier (1-10 characters, non-null, non-empty)
     * @param firstName   first name (1-10 characters, non-null, non-empty)
     * @param lastName    last name (1-10 characters, non-null, non-empty)
     * @param phone       phone number (exactly 10 digits, no formatting, non-null)
     * @param address     address (1-30 characters, non-null, non-empty)
     * @throws IllegalArgumentException if any field violates its constraints
     */
    public Contact(String contactID, String firstName, String lastName, String phone, String address) {
        
    	// Validate contactID
        if (contactID == null || contactID.length() > 10 || contactID.isEmpty()) {
            throw new IllegalArgumentException("Invalid contact ID: must be non-null, non-empty, and <= 10 characters");
        }

        this.contactID = contactID;

        // Set fields with validation
        setFirstName(firstName);
        setLastName(lastName);
        setPhone(phone);
        setAddress(address);
    }

    
    /**
     * Returns the immutable contact ID.
     *
     * @return the contact ID
     */
    public String getContactID() {
        return contactID;
    }

    
    /**
     * Returns the first name.
     *
     * @return the first name
     */
    public String getFirstName() {
        return firstName;
    }

    
    /**
     * Updates the first name if valid.
     *
     * @param firstName new first name (1-10 characters, non-null, non-empty)
     * @throws IllegalArgumentException if the value is invalid
     */
    public void setFirstName(String firstName) {
        if (firstName == null || firstName.length() > 10 || firstName.isEmpty()) {
            throw new IllegalArgumentException("Invalid first name: must be non-null, non-empty, and <= 10 characters");
        }
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    
    /**
     * Updates the last name if valid.
     *
     * @param lastName new last name (1-10 characters, non-null, non-empty)
     * @throws IllegalArgumentException if the value is invalid
     */
    public void setLastName(String lastName) {
        if (lastName == null || lastName.length() > 10 || lastName.isEmpty()) {
            throw new IllegalArgumentException("Invalid last name: must be non-null, non-empty, and <= 10 characters");
        }
        this.lastName = lastName;
    }

    public String getPhone() {
        return phone;
    }

    
    /**
     * Updates the phone number if it is exactly 10 digits.
     *
     * @param phone new phone number (exactly 10 digits, no formatting)
     * @throws IllegalArgumentException if not exactly 10 digits
     */
    public void setPhone(String phone) {
        if (phone == null || !phone.matches("\\d{10}")) {
            throw new IllegalArgumentException("Invalid phone: must be exactly 10 digits (no spaces, dashes, etc.)");
        }
        this.phone = phone;
    }

    public String getAddress() {
        return address;
    }

    
    /**
     * Updates the address if valid.
     *
     * @param address new address (1-30 characters, non-null, non-empty)
     * @throws IllegalArgumentException if the value is invalid
     */
    public void setAddress(String address) {
        if (address == null || address.length() > 30 || address.isEmpty()) {
            throw new IllegalArgumentException("Invalid address: must be non-null, non-empty, and <= 30 characters");
        }
        this.address = address;
    }
}
