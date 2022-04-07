#include <utl.hpp>
// #include <iostream>
// #include <filesystem>




// class Zalupa{};
// int main()
// {
//     //------------------String util test begin------------------
//     /*std::cout << "------------------String util test begin------------------" << std::endl;
//     auto var = 10;
//     auto converted = utl::string_util::conv(var);
//     auto casted = utl::string_util::cast<int>(converted);
//     std::cout << var << " " << utl::string_util::name_of(var) << converted << " " 
//     << utl::string_util::name_of(converted) << casted << " " 
//     << utl::string_util::name_of(casted) << " " 
//     << utl::string_util::name_of<Zalupa>() << std::endl;
//     std::cout << "------------------String util test end------------------" << std::endl;*/
//     //------------------String util test end------------------

//     //------------------Reactive variable test begin------------------
//     std::cout << "------------------Reactive variable test begin------------------" << std::endl;
//     utl::rint32_t penis;
//     penis.bind([&](const int32_t& a, const int32_t& b){
//         std::cout << a << b << std::endl;
//     });
//     penis = 1488;

//     utl::rstring rstr;
//     rstr.bind([&](const std::string& a, const std::string& b){
//         std::cout << a << b << std::endl;
//     });
//     rstr = std::string("hello world");
//     std::cout << "------------------Reactive variable test end------------------" << std::endl;
//     //------------------Reactive variable test end------------------


//     //------------------Custom allocator for unique_ptr test begin------------------
//     std::cout << "------------------Custom allocator for unique_ptr test begin------------------" << std::endl;
//     auto pointer = utl::make_unique<utl::vector<utl::string>>();
//     std::cout << "------------------Custom allocator for unique_ptr test end------------------" << std::endl;
//     //------------------Custom allocator for unique_ptr test end------------------


//     //------------------Thread pool test begin------------------
//     std::cout << "------------------Thread pool test begin------------------" << std::endl;
//     auto pool = utl::threadpool();
//     utl::vector<std::future<uint32_t>> results{};
//     for(uint32_t t = 0; t < 1000; t++) results.emplace_back(pool.submit([=](const uint32_t a, const uint32_t b, const uint32_t c) -> uint32_t
//     { 
//         return (a*1000/10)*(120 + b*t+c)/(a+b*c)+1; 
//     }, static_cast<uint32_t>(t+10), static_cast<uint32_t>(t+20), static_cast<uint32_t>(t*20)));
//     pool.wait();

//     uint32_t sum{0};
//     for(auto& res : results)
//     {
//         sum += res.get();
//     }
//     std::cout << "Future result: " << sum << std::endl;
//     std::cout << "------------------Thread pool test end------------------" << std::endl;
//     //------------------Thread pool test end------------------

//     return 0;
// }

#include <cstdint>
#include <functional>
#include <queue>
#include <memory>
#include <vector>
#include <iostream>
#include <mutex>
#include <map>
#include <atomic>
#include <chrono>
#include "zipextractor.hpp"

struct FTaskData
{
    enum class ETaskStatus
    {
        eIdle,
        eInProgress,
        eFinished
    };

    enum class ETaskType
    {
        eDownloader,
        eUnpacker,
        eFister
    };

    int32_t finished, total;
    uint32_t game_id;
    std::string uri;
    fs::path savepath;
    ETaskStatus status;
    ETaskType type;
};


class IObservable
{
public:
    virtual ~IObservable() {}
    virtual void notify(FTaskData) = 0;
};

class IObserver
{
public:
    void update(FTaskData data)
    {
        for(auto& observer : m_vObservers)
            observer->notify(data);
    }
protected:
    std::vector<std::shared_ptr<IObservable>> m_vObservers;
};

class Task : public IObserver
{
public:
    void begin()
    {
        task_data.status = FTaskData::ETaskStatus::eIdle; notify();
        task_data.status = FTaskData::ETaskStatus::eInProgress; 

        try
        {
            do_work();
        }
        catch(...)
        {
            std::rethrow_exception(std::current_exception());
        }

        task_data.status = FTaskData::ETaskStatus::eFinished;
    }

    virtual void do_work() = 0;

    void pause() { m_bIsPaused = true; }
    void resume() { m_bIsPaused = false; }

    void notify() { update(task_data); }
protected:
    FTaskData task_data;
    Task(std::shared_ptr<IObservable> pObservable, FTaskData data)
    {
        task_data = data;
        m_vObservers.emplace_back(pObservable);
    }

    std::atomic<bool> m_bIsPaused{false};
};

class DownloadingTask : public Task
{
public:
    DownloadingTask(std::shared_ptr<IObservable> pObservable, FTaskData data) : Task(pObservable, data) 
    { task_data.type = FTaskData::ETaskType::eDownloader; }

    void do_work() override
    {
        for(auto i = 0; i < 500; i++)
        {
            //Pause work
            while(m_bIsPaused);

            task_data.finished = i; task_data.total = 500; notify();
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
    }
};

class UnpackingTask : public Task
{
public:
    UnpackingTask(std::shared_ptr<IObservable> pObservable, FTaskData data) : Task(pObservable, data) 
    { task_data.type = FTaskData::ETaskType::eUnpacker; }

    void do_work() override
    {
        for(auto i = 0; i < 500; i++)
        {
            //Pause work
            while(m_bIsPaused.load(std::memory_order::acquire));

            task_data.finished = i; task_data.total = 500; notify();
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
    }
};

class Container
{
public:
    void begin()
    {
        while(true)
        {
            if(m_qTasks.empty())
                return;

            m_pCurrentTask = std::move(m_qTasks.front());
            m_pCurrentTask->begin();
            m_qTasks.pop();
            m_pCurrentTask->notify();
        }
    }

    void push(std::unique_ptr<Task>&& newTask) { m_qTasks.emplace(std::move(newTask)); }

    void pause() { m_pCurrentTask->pause(); }
    void resume() { m_pCurrentTask->resume(); }

    bool is_valid() 
    { 
        int penis = 0;
        return !m_qTasks.empty();
    }
private:
    std::queue<std::unique_ptr<Task>> m_qTasks;
    std::unique_ptr<Task> m_pCurrentTask;
};

class DownloadingManager : public IObservable, public std::enable_shared_from_this<DownloadingManager>
{
public:
    ~DownloadingManager()
    {
        
    }

    void begin_download(FTaskData data, const std::string& str)
    {
        //check task status on config
        auto task_container = std::make_shared<Container>();
        task_container->push(std::make_unique<DownloadingTask>(shared_from_this(), data));
        task_container->push(std::make_unique<UnpackingTask>(shared_from_this(), data));

        tasks.emplace(data.game_id, task_container);
        pool.push([=]()
        { 
            auto it = tasks.find(data.game_id);
            if(it != tasks.end())
                it->second->begin(); 
        });
    }

    void pause(uint32_t game_id)
    {
        auto it = tasks.find(game_id);
        if(it != tasks.end())
            it->second->pause();
    }

    void resume(uint32_t game_id)
    {
        auto it = tasks.find(game_id);
        if(it != tasks.end())
            it->second->resume();
    }

    void stop(uint32_t game_id)
    {
        auto it = tasks.find(game_id);
        if(it != tasks.end())
            tasks.erase(it);
    }

    void notify(FTaskData data) override
    {
        {
            std::unique_lock<std::mutex> _lock(callback_mutex);
            std::cout << "[" << data.game_id << "] " << static_cast<int>(data.status) << ":" << data.finished << " | " << static_cast<int>(data.type) << std::endl;
            /*auto it = tasks.find(data.id);
            if(it != tasks.end())
                if(!it->second->is_valid())
                    tasks.erase(it);*/
        }

        //thread safe code here


        //update config

        //call all subscribers
    }

    void wait()
    {
        pool.wait();
    }
private:
    std::map<uint32_t, std::shared_ptr<Container>> tasks;
    utl::threadpool pool;
    std::mutex callback_mutex;
};

int main()
{
    /*auto manager = std::make_shared<DownloadingManager>();
    manager->begin_download(1001, "jkaflfa");
    manager->begin_download(1002, "hfokashnopfisd");
    manager->begin_download(1488, "lkshfal");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    manager->pause(1001);
    manager->pause(1002);
    manager->pause(1488);
    std::cout << "paused" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    manager->resume(1001);
    manager->resume(1002);
    manager->resume(1488);
    std::cout << "resumed" << std::endl;

    manager->wait();*/
    zipextractor::extract("C:\\Users\\AdamFull\\Documents\\GitHub\\utllib\\bin\\debug\\Memory_Manager.zip", "C:\\Users\\AdamFull\\Documents\\GitHub\\utllib\\bin\\debug");
    return 0;

}