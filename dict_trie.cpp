#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define ALPHABET_SIZE 26

struct TrieNode
{
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    string value;
};

int letterToIndex(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
        return letter - 'A';
    else if (letter >= 'a' && letter <= 'z')
        return letter - 'a';
    return -1;
}

TrieNode* getNode()
{
    TrieNode* pNode = new TrieNode;
    pNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = nullptr;
    return pNode;
}

bool insert(TrieNode* root, const string& word, const string& meaning)
{
    TrieNode* pCrawl = root;
    for (char letter : word)
    {
        int index = letterToIndex(letter);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
    pCrawl->value = meaning;
    return true;
}

int readWordsFromFile(TrieNode* root, const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Could not find/open file \"" << filename << "\"\n";
        return false;
    }

    string word, meaning;
    int count = 0;

    while (file >> word && getline(file, meaning))
    {
        if (!insert(root, word, meaning))
        {
            file.close();
            return false;
        }
        else
        {
            count++;
        }
    }

    file.close();
    cout << "Welcome to our Dictionary. Our Dictionary right now contains " << count << " words.\n";
    return true;
}

bool search(TrieNode* root, const string& word)
{
    TrieNode* pCrawl = root;
    for (char letter : word)
    {
        int index = letterToIndex(letter);
        if (!pCrawl->children[index])
            return false;
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
    {
        cout << "The meaning of the word is:\n" << pCrawl->value;
        return true;
    }
    return false;
}

bool deleteWord(TrieNode* root, const string& word)
{
    TrieNode* pCrawl = root;
    for (char letter : word)
    {
        int index = letterToIndex(letter);
        if (!pCrawl->children[index])
            return false;
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
    {
        pCrawl->isEndOfWord = false;
        return true;
    }
    return false;
}

void alphabeticalOrder(TrieNode* root, char allWords[], int index)
{
    TrieNode* pCrawl = root;
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
    {
        for (int j = 0; j < index; j++)
            cout << allWords[j];
        cout << ": " << pCrawl->value << "\n";
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (pCrawl->children[i] != nullptr)
        {
            allWords[index] = 'a' + i;
            alphabeticalOrder(pCrawl->children[i], allWords, index + 1);
        }
    }
}

void printPrefixSearch(TrieNode* root, char allWords[], int index, const string& prefix)
{
    TrieNode* pCrawl = root;
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
    {
        cout << prefix;
        for (int j = 0; j < index; j++)
            cout << allWords[j];
        cout << ": " << pCrawl->value << "\n";
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (pCrawl->children[i] != nullptr)
        {
            allWords[index] = 'a' + i;
            printPrefixSearch(pCrawl->children[i], allWords, index + 1, prefix);
        }
    }
}

void prefixSearch(TrieNode* root, const string& prefix)
{
    TrieNode* pCrawl = root;
    for (char letter : prefix)
    {
        int index = letterToIndex(letter);
        pCrawl = pCrawl->children[index];
    }
    char allWords[50];
    printPrefixSearch(pCrawl, allWords, 0, prefix);
}

bool searchSuggestedWord(TrieNode* root, const string& word, int count)
{
    TrieNode* pCrawl = root;
    for (char letter : word)
    {
        int index = letterToIndex(letter);
        if (!pCrawl->children[index])
            return false;
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl != nullptr && pCrawl->isEndOfWord)
    {
        if (count == 0)
            cout << "Suggested words are: ";
        cout << word << " ";
    }
    return true;
}

bool suggestedWords(TrieNode* root, const string& word, int number)
{
    int count = 0;
    for (size_t i = 0; i < word.length(); i++)
    {
        string temp = word;
        for (int j = 0; j < ALPHABET_SIZE; j++)
        {
            if (count < number && j != letterToIndex(word[word.length() - i - 1]))
            {
                temp[word.length() - i - 1] = 'a' + j;
                if (searchSuggestedWord(root, temp, count))
                    count++;
            }
        }
    }
    if (count == 0)
    {
        cout << "\nSorry we couldn't find any words related to " << word << " in our Dictionary.\n";
        return false;
    }
    else if (count < number)
    {
        cout << "\nWe could only find " << count << " words related to " << word << " in our Dictionary.\n";
        return true;
    }
    else
    {
        return true;
    }
}

int main()
{
    TrieNode* root = getNode();
    readWordsFromFile(root, "words.txt");

    string command;
    do
    {
        cout << "\nWhat would you like to do?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix search.\n";
        cout << "6. Show suggested words.\n\n";
        fflush(stdin);
        int call;
        cin >> call;
        cout << "\n";
        string word, meaning;
        switch (call)
        {
            case 1:
                cout << "Enter the word you would like to insert: ";
                cin >> word;
                cout << "Enter its meaning: ";
                cin.ignore();
                getline(cin, meaning);
                if (insert(root, word, meaning))
                    cout << word << " has been added to the Dictionary.\n";
                break;
            case 2:
                cout << "Enter the word you would like to search: ";
                cin >> word;
                if (!search(root, word))
                {
                    cout << "Sorry, the word you searched for doesn't exist. Would you like to add it to the Dictionary? (Yes/No) ";
                    string add;
                    cin >> add;
                    if (add == "yes" || add == "Yes" || add == "YES")
                    {
                        cout << "Enter its meaning: ";
                        cin.ignore();
                        getline(cin, meaning);
                        if (insert(root, word, meaning))
                            cout << word << " has been added to the Dictionary.\n";
                    }
                }
                break;
            case 3:
                cout << "Which word would you like to delete? ";
                cin >> word;
                if (deleteWord(root, word))
                    cout << word << " has been successfully deleted from the Dictionary.\n";
                else
                    cout << "No such word exists in the Dictionary.\n";
                break;
            case 4:
                char allWords[50];
                alphabeticalOrder(root, allWords, 0);
                cout << "\n";
                break;
            case 5:
                cout << "Enter the word you would like to use as a prefix: ";
                cin >> word;
                cout << "\n";
                prefixSearch(root, word);
                break;
            case 6:
                cout << "Enter the word for whose suggested words you want to see: ";
                cin >> word;
                int number;
                cout << "How many suggested words do you want to see: ";
                cin >> number;
                suggestedWords(root, word, number);
                break;
            default:
                cout << "Enter a valid entry.";
        }
        fflush(stdin);
        cout << "\n\nWould you like to continue or exit? (Yes/No) ";
        cin >> command;
    } while (command == "yes" || command == "Yes" || command == "YES");
    cout << "Thanks for using our Dictionary.\n";
    return 0;
}