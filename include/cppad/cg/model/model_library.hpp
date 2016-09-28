#ifndef CPPAD_CG_MODEL_LIBRARY_INCLUDED
#define CPPAD_CG_MODEL_LIBRARY_INCLUDED
/* --------------------------------------------------------------------------
 *  CppADCodeGen: C++ Algorithmic Differentiation with Source Code Generation:
 *    Copyright (C) 2013 Ciengis
 *
 *  CppADCodeGen is distributed under multiple licenses:
 *
 *   - Eclipse Public License Version 1.0 (EPL1), and
 *   - GNU General Public License Version 3 (GPL3).
 *
 *  EPL1 terms and conditions can be found in the file "epl-v10.txt", while
 *  terms and conditions for the GPL3 can be found in the file "gpl3.txt".
 * ----------------------------------------------------------------------------
 * Author: Joao Leal
 */

namespace CppAD {
namespace cg {

enum class ThreadPoolScheduleStrategy {
    SINGLE_JOB, // each thread only executes a single job at a time
    MULTI_JOB // each thread can execute multiple jobs before returning to the pool
};

/**
 * Abstract class used to load models
 * 
 * @author Joao Leal
 */
template<class Base>
class ModelLibrary {
public:
    /**
     * Provides the model names in the dynamic library.
     * 
     * @return the model names
     */
    virtual std::set<std::string> getModelNames() = 0;

    /**
     * Creates a new GenericModel object that can be used to evaluate the
     * model.
     * This object must be released by the user!
     * 
     * @param modelName The model name.
     * @return The model object (must be released by the user) or nullptr if 
     *         no model exists with the provided name 
     */
    virtual GenericModel<Base>* model(const std::string& modelName) = 0;

    virtual void setThreadPoolDisabled(bool disabled) = 0;

    /**
     * Provides the maximum number of threads used to determine sparse Jacobians
     * and sparse Hessians for the models in this library.
     * This value is only used by the models if they were compiled with
     * multithreading support.
     *
     * @return the maximum number of threads
     */
    virtual unsigned int getThreadNumber() const = 0;

    /**
     * Defines the maximum number of threads used to determine sparse Jacobians
     * and sparse Hessians for the models in this library.
     * This value is only used by the models if they were compiled with
     * multithreading support.
     * It should be defined before using the models.
     *
     * @param n the maximum number of threads
     */
    virtual void setThreadNumber(unsigned int n) = 0;

    /**
     * Provides the thread scheduling strategy used to determine sparse Jacobians
     * and sparse Hessians for the models in this library.
     * This value is only used by the models if they were compiled with
     * multithreading support.
     *
     * @return the thread scheduling strategy
     */
    virtual ThreadPoolScheduleStrategy getThreadPoolSchedulerStrategy() const = 0;

    /**
     * Defines the thread scheduling strategy used to determine sparse Jacobians
     * and sparse Hessians for the models in this library.
     * This value is only used by the models if they were compiled with
     * multithreading support.
     * It should be defined before using the models.
     *
     * @param s the thread scheduling strategy
     */
    virtual void setThreadPoolSchedulerStrategy(ThreadPoolScheduleStrategy s) = 0;

    virtual void setThreadPoolVerbose(bool v) = 0;

    virtual bool isThreadPoolVerbose() = 0;

    virtual void setThreadPoolMultiJobMaxWork(float v) = 0;

    virtual float getThreadPoolMultiJobMaxWork() = 0;

    inline virtual ~ModelLibrary() {
    }

};

} // END cg namespace
} // END CppAD namespace

#endif