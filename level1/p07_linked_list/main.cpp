#include<iostream>
using namespace std;

template <typename T>
struct Node {
    T data;        
    Node* next;   

    Node(const T& val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;     
    size_t size;       

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() { clear(); }

    // 头插法
    void insertAtHead(const T& val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
        ++size;
    }

    // 尾插法
    void insertAtTail(const T& val) {
        if (!head) {
            insertAtHead(val);
            return;
        }
        Node<T>* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = new Node<T>(val);
        ++size;
    }

    // 按索引插入
    bool insertAtIndex(size_t index, const T& val) {
        if (index > size) return false;
        if (index == 0) {
            insertAtHead(val);
            return true;
        }
        Node<T>* prev = head;
        for (size_t i = 0; i < index - 1; ++i)
            prev = prev->next;

        Node<T>* newNode = new Node<T>(val);
        newNode->next = prev->next;
        prev->next = newNode;
        ++size;
        return true;
    }

    // 删除指定位置
    bool deleteAtIndex(size_t index) {
        if (index >= size) return false;

        if (index == 0) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            --size;
            return true;
        }

        Node<T>* prev = head;
        for (size_t i = 0; i < index - 1; ++i)
            prev = prev->next;

        Node<T>* temp = prev->next;
        prev->next = temp->next;
        delete temp;
        --size;
        return true;
    }

    // 查找
    bool find(const T& val) const {
        Node<T>* cur = head;
        while (cur) {
            if (cur->data == val) return true;
            cur = cur->next;
        }
        return false;
    }

    // 遍历打印
    void print() const {
        Node<T>* cur = head;
        while (cur) {
            std::cout << cur->data << " -> ";
            cur = cur->next;
        }
        std::cout << "nullptr\n";
    }

    // 清空链表
    void clear() {
        Node<T>* cur = head;
        while (cur) {
            Node<T>* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
        size = 0;
    }
};


int main() {

    LinkedList<int> list;

  
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.print();

    
    list.insertAtHead(5);
    list.print();


    list.insertAtIndex(2, 15);
    list.print();

  
    list.deleteAtIndex(3);
    list.print();

    // 查找测试
    int target = 20;
    if(list.find(target)){
        cout << "\n找到元素 " << target << endl;
    }else{
        cout << "\n未找到元素 " << target << endl;
    }

    cout << "\n==== 测试string类型模板链表（模板通用性） ====" << endl;
    LinkedList<string> strList;
    strList.insertAtTail("hello");
    strList.insertAtTail("world");
    strList.print();

    return 0;
}