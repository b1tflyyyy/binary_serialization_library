The Cpp Binary Serialization Library is a software package that enables users to serialize and deserialize data in binary format. 
It is compatible with both C++ 17 and 20 versions, and its functionality is encapsulated in the SerializationBinary class, which resides in the szon namespace. Users can create an instance of the class by instantiating an object of the szon::SerializationBinary type.

The library provides a range of methods for serializing and deserializing data, including SerializeArrayOfData, DeserializeArrayOfData, SerializeData, and DeserializeData. 
The SerializeArrayOfData method allows users to serialize an array of data of any data type, while the DeserializeArrayOfData method allows them to deserialize data into the array passed as the first parameter. 
The SerializeData method enables the serialization of a single variable of any data type, while the DeserializeData method deserializes data into the variable passed as the first parameter.

Other methods available in the library include IsFileEmpty, which returns true if the file is empty and false if there is any data in the file; ResetFileReadPosition, which resets the position at which file reading starts and should be used after deserializing all data; and ClearFile, which erases all data from the file.

More information can be found in the documentation folder.

You can see usage examples at this link https://github.com/b1tflyyyy/binary_serialization_example
