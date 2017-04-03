#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H

class AbstractTask {
public:
  virtual void init() = 0;

  virtual void update() = 0;
    
};

#endif /* ABSTRACTTASK_H */

