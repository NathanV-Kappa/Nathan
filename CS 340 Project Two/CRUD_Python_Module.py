from pymongo import MongoClient
from pymongo.errors import PyMongoError

class AnimalShelter(object):
    # CRUD operations for Animal collection in MongoDB

    def __init__(self, username, password):
        # Initializing the MongoClient. This helps to access the MongoDB databases and collections. Hard-coded to use the aac database, the animals collection, and the aacuser.
                
        # Connection Variables
        USER = username
        PASS = password
        HOST = 'localhost'
        PORT = '27017'
        DB = 'AAC'
        COL = 'animals'

        # Initialize Connection
        try:
            self.client = MongoClient(
                f'mongodb://{USER}:{PASS}@{HOST}:{PORT}/?authSource=admin'
            )
            self.database = self.client[DB]
            self.collection = self.database[COL]
            # Test connection
            self.client.admin.command('ping')
            print("Connected to MongoDB successfully.")
            
        except PyMongoError as e:
            print(f"Connection failed: {e}")
            raise

    def create(self, data):
        # Created method to implement the C in CRUD.
        # Create method - inserts one document.
        if data is not None:
            try:
                result = self.collection.insert_one(data)
                return result.acknowledged
            
            except PyMongoError as e:
                print(f"Insert error: {e}")
                return False
            
        else:
            raise Exception("Nothing to save, because data parameter is empty")

    def read(self, query):
        # Created method to implement the R in CRUD.
        # Read method - returns list of documents matching query.
        try:
            cursor = self.collection.find(query)
            return list(cursor)
        
        except PyMongoError as e:
            print(f"Query error: {e}")
            return []
    
    def update(self, query, update_data):
        # Created method to implement the U in CRUD.
        # Update method - updates document(s) matching query. Returns the number of documents modified.
        try:
            result = self.collection.update_many(query, {"$set": update_data})
            return result.modified_count
        
        except PyMongoError as e:
            print(f"Update error: {e}")
            return 0

    def delete(self, query):
        # Created method to implement the D in CRUD.
        # Delete method - removes document(s) matching query. Returns the number of documents deleted.
        try:
            result = self.collection.delete_many(query)
            return result.deleted_count
        
        except PyMongoError as e:
            print(f"Delete error: {e}")
            return 0