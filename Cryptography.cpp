#include<iostream>
#include<random>
#include<ctime>
#include<fstream>
#include<cstring>
#include<cstdlib>

using namespace std;

struct Key {
    int key1,key2,key3;
    char filekey[10];
};

class Cryptography{
    private:
        Key key;
    public:
        Cryptography() {
            Key key;
            key.key1 = rand() % 1000 + 1;
            key.key2 = rand() % 1000 + 1;
            key.key3 = rand() % 1000 + 1;
            for(int i = 0; i < 10; i++) {
                key.filekey[i] = 'A' + rand() % 26; // Random uppercase letters
            }
            if(system("dir /b Keys") != 0) {
                cout << "Creating Keys directory..." << endl;
                system("mkdir Keys");
            }
            ofstream keyFile("./Keys/_Key.bin", ios::binary);
            if (keyFile.is_open()) {
                keyFile.write(reinterpret_cast<char*>(&key), sizeof(Key));
                keyFile.close();
                cout << "Key generated and saved successfully." << endl;
            } else {
                cout << "Error opening file to save key." << endl;
            }
        }

        void encrypt(const string& inputfile,const string& KeyFile,const string& outputfile) {
            Key key;
            ifstream keyInput(KeyFile, ios::binary);
            ifstream input(inputfile);
            ofstream output(outputfile);//create a file
            if (!keyInput.is_open()) {
                cerr << "Error: Unable to open key file for decryption. Make sure it exists and is accessible." << endl;
                return;
            }
            if (!input.is_open()) {
                cerr << "Error: Unable to open input file '" << inputfile << "' for decryption. Make sure it exists and is accessible." << endl;
                keyInput.close();
                return;
            }
            if (!output.is_open()) {
                cerr << "Error: Unable to open output file '" << outputfile << "' for decryption. Check permissions or if the path is valid." << endl;
                keyInput.close();
                input.close();
                return;
            }
            keyInput.read(reinterpret_cast<char*>(&key), sizeof(Key));
            keyInput.close();
            char ch;
            int l =1;
            while (input.get(ch)) {
                // Simple XOR encryption
                if(l==1) {
                    ch -= key.key1;
                } else if(l==2) {
                    ch -= key.key2;
                } else if(l==3) {
                    ch -= key.key3;
                    l = 0; // Reset to 1 after using key3
                }
                l++;
                output.put(ch);
            }
            input.close();
            output.close();
            cout << "key: " << key.filekey << endl;
            cout << "File encrypted successfully." << endl;
        }

        void decrypt(const string& inputfile,const string& KeyFile) {
            Key key;
            ifstream keyInput(KeyFile, ios::binary);
            ifstream input(inputfile);
            if (!keyInput.is_open()) {
                cerr << "Error: Unable to open key file for decryption. Make sure it exists and is accessible." << endl;
                return;
            }
            if (!input.is_open()) {
                cerr << "Error: Unable to open input file '" << inputfile << "' for decryption. Make sure it exists and is accessible." << endl;
                keyInput.close();
                return;
            }
            keyInput.read(reinterpret_cast<char*>(&key), sizeof(Key));
            keyInput.close();
            /*cout<< "Enter the file key: ";
            char inputKey[10];
            cin >> inputKey;
            if (strcmp(inputKey, key.filekey) != 0) {
                cout << "Incorrect file key. Decryption failed." << endl;
                return;
            }*/
            char ch;
            int l = 1;
            string ab;
            while (input.get(ch)) {
                if(l==1) {
                    ch += key.key1;
                } else if(l==2) {
                    ch += key.key2;
                } else if(l==3) {
                    ch += key.key3;
                    l = 0; // Reset to 1 after using key3
                }
                l++;
                ab+=ch;
            }
            input.close();
            cout << "File decrypted successfully." << endl;          
            cout << "Decrypted content: " << ab << endl;
        }

        void encrypt_decrypt_sentence(const string& sentence, const string& KeyFile) {
            ifstream keyInput(KeyFile, ios::binary);
            if (!keyInput.is_open()) {
                cerr << "Error: Unable to open key file for encryption/decryption. Make sure it exists and is accessible." << endl;
                return;
            }
            keyInput.read(reinterpret_cast<char*>(&key), sizeof(Key));
            keyInput.close();
            string ecr_sen;
            char ch;
            for(int i=0;i< sentence.length();i++) {
                ch = sentence[i];
                if(i % 3 == 0) {
                    ch += key.key1;
                } else if(i % 3 == 1) {
                    ch += key.key2;
                } else {
                    ch += key.key3;
                }
                ecr_sen += ch;
            }
            cout << "Encrypted sentence: " << ecr_sen << endl;

            for(int i = 0; i < ecr_sen.length(); i++) {
                ch = ecr_sen[i];
                if(i % 3 == 0) {
                    ch -= key.key1;
                } else if(i % 3 == 1) {
                    ch -= key.key2;
                } else {
                    ch -= key.key3;
                }
                ecr_sen[i] = ch;
            }
            cout << "Decrypted sentence: " << ecr_sen << endl;
        }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    try{
        Cryptography crypto;
        string inputfile, outputfile;
        cout << "Enter the input file to encrypt: ";
        cin >> inputfile;
        cout << "Enter the output file for encryption: ";
        cin >> outputfile;
        crypto.encrypt(inputfile,"Keys/_Key.bin", outputfile);
        cout << "Enter the new output file to see decryption: ";
        string decryptOutputFile;
        cin >> decryptOutputFile;
        crypto.decrypt(outputfile, "Keys/_Key.bin");
        cout << "Decryption completed. Check the output file." << endl;
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    } catch (...) {
        cerr << "An unknown error occurred." << endl;
    }
    /*try{
        Cryptography crypto;
        string sentence, keyFile;
        cout << "Enter a sentence to encrypt/decrypt: ";
        getline(cin, sentence);
        crypto.encrypt_decrypt_sentence(sentence, "Keys/_Key.bin");
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    } catch (...) {
        cerr << "An unknown error occurred." << endl;
    }*/


    return 0;
}