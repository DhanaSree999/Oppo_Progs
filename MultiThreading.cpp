#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <vector>
#include <cmath> 

std::mutex mtx;
std::condition_variable cv;
int currentThread = 1;
int crntwrd=0;

// Function to split sentence into words
std::vector<std::string> splitSentence(const std::string& sentence) {
    std::istringstream iss(sentence);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

// Function to be executed by each thread
void threadFunction(const std::vector<std::string>& words, int threadID,int nt) {
    // Lock the mutex to access shared resources
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until it's the thread's turn to print
    cv.wait(lock, [threadID]{ return threadID == currentThread; });

    std::cout << "Thread " << threadID << ": ";

    // Print words from the sentence for the current thread
    for (int i = crntwrd; i < crntwrd+5 && i < words.size(); ++i) {
        std::cout << words[i] << " ";
    }
    crntwrd+=5;
    std::cout << std::endl;

    // Update the currentThread value for the next thread
    currentThread = (currentThread % nt) + 1;

    // Notify all threads to proceed
    cv.notify_all();
}

int main() {
   int numThreads=5;
    
    // Input sentence
    //std::string sentence = "SurfaceFlinger is an integral component of the Android operating system. It's responsible for compositing and rendering graphical content to the screen. Essentially, it manages the rendering of all graphical elements, including app windows, widgets, and the user interface and all other cases";
    std::string sentence;
    
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, sentence); // Get the entire line of input

    // Convert sentence to array of words
    std::vector<std::string> words = splitSentence(sentence);
    float wrdcnt=words.size();
    std::cout<<wrdcnt<<std::endl;
    int nt=ceil(wrdcnt/numThreads);
   
    std::thread threads[nt];
    

    // Create multiple threads
    
    for (int i = 0; i < nt; ++i) 
    {
        threads[i] = std::thread(threadFunction, std::cref(words), i + 1,nt);
    }

    // Join threads to the main thread
    for (int i = 0; i < nt; ++i) 
    {
        threads[i].join();
    }

    return 0;
}
