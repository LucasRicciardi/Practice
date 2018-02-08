// io includes
#include <iostream>

/*
*   1 - Define a non-concurrent queue abstraction that implements
*   a bounded buffer - using an internal representation like a circular
*   array or linked list. This implementation is not concerned with mutual
*   exclusion or condition synchronization. The following MessageQueueRep
*   class presents the interface for this queue:
*/

// The template parameter T corresponds to the type of messages stored in the queue:
template <typename MessageType>
class MessageQueueRep {
public:
    void enqueue(MessageType message);
    MessageType dequeue(void);

    bool empty(void) const;
    bool full(void) const;

private:
    // Internal resource representation
};


/*
*   2 - Define a Scheduler that enforces the particular mutual exclusion
*   and condition synchronization constraints - The Scheduler determines the
*   order to process methods based on synchronization constraints. These constraints
*   depend on the state of the resource being represented. For example, if the
*   MessageQueueRep is used to implement an Output Handler, these constraints
*   would indicate whether the queue was empty, full or neither.
*/

class MethodObject {
protected:
    virtual bool guard() const;
    virtual void call();
};

template <typename T>
class MessageQueueScheduler {
protected:
    class Enqueue: public MethodObject {
    public:
        Enqueue(MessageQueueRep<T> * rep, T args):
            mRep(rep),
            mArg(arg)
        {}

        virtual bool guard(void) const {
            // Synchronization constraint
            return not mRep->full();
        }

        virtual void call() {
            // Insert message into message queue
            mRed->enqueue(mArg);
        }
    private:
        MessageQueueRep<T> * mRep;
        T mArg;
    };
};

class Dequeue: public MethodObject {
public:
    Dequeue(MessageQueueRep<T> * rep, Future<T> &f):
        mRep(rep),
        mResult(f)
    {}

    bool guard() const {
        // Synchronization constraint
        return not mRep->empty();
    }

    virtual void call(void) {
        // Bind the removed message to the future result object
        mResult = mRep->dequeue();
    }

private:
    MessageQueueRep<T> * mRep;

    // Future message result value
    Future<T> mResult;
};

int main() {

    return 0;
}
