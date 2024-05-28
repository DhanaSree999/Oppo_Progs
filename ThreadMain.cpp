//Write a process from which you spawn 2 threads ,use mutex to write in global file 
//Take 2 strings or sentences(100 words each) and each thread has to print 5 words line by line 
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <sstream>

std::mutex mtx;
std::condition_variable cv;
bool thread1_done = false;

// Function for task 1
void task1(const std::string& sentence) {
    std::istringstream iss(sentence);
    std::string word;
    int wordIndex = 0;
    int numWords1 = 0;
    while (iss >> word) {
        ++numWords1;
    }
    iss.clear();
    iss.seekg(0);
    while (wordIndex < numWords1) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [](){ return !thread1_done; }); // Wait until thread 2 is done
        std::cout << "Thread 1: ";
        for (int i = 0; i < 5 && iss >> word; ++i) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
        thread1_done = true;
        cv.notify_one(); // Notify thread 2 to start
        wordIndex += 5;
    }
}


// Function for task 2
void task2(const std::string& sentence) {
    std::istringstream iss(sentence);
    std::string word;
     int wordIndex = 0;
    int numWords2 = sentence.size();
     while (wordIndex < numWords2) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [](){ return thread1_done; }); // Wait until thread 1 is done
        std::cout << "Thread 2: ";
       for (int i = 0; i < 5 && iss >> word; ++i) 
       {
       
            std::cout << word << " ";
        }
         
         wordIndex += 5;
        std::cout << std::endl;
        thread1_done = false;
        cv.notify_one(); // Notify thread 1 to start
    }
}

int main() {
    std::string sentence1 = "SurfaceFlinger is an integral component of the Android operating system. It's responsible for compositing and rendering graphical content to the screen. It manages the rendering of all graphical elements, including app windows, widgets, and the user interface.Every window that is created on the Android platform is backed by a surface. All of the visible surfaces rendered are composited onto the display by SurfaceFlinger.For example, let’s say you have a game running in the device background, a music player in the middle, and notifications popping up on top. SurfaceFlinger makes sure that the game graphics don't interfere with music player.";
    std::string sentence2 = "A BufferQueue is a data structure used to manage a sequence of data buffers, often in a producer-consumer model. BufferQueue facilitates the orderly transfer of data between producers (those generating or providing data) and consumers (those processing or consuming data).The BufferQueue class connects components that generate buffers of graphical data (producers) to components that accept the data for display or further processing (consumers).BufferQueues are efficient for managing data flow between different parts of a system because they provide a structured way to handle the transfer of data, avoiding issues like data loss, deadlock etc. The sentence 2 ends here .";

    std::thread t1(task1, sentence1);
    std::thread t2(task2, sentence2);

    t1.join(); // Wait for thread 1 to finish
    t2.join(); // Wait for thread 2 to finish

    return 0;
}
