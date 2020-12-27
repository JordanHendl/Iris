#ifndef Karma_INTERFACE_H
#define Karma_INTERFACE_H

/** Object to interface and interact with the Karma Library.
 */
class Karma
{
  public:
    
    /** Default Constructor.
     */
    Karma() ;
    
    /** Deconstructor.
     */
    ~Karma() ;
    
    /** Method to initialize the Karma Library.
     * @note The base path is used as a basis for all Asset & JSON lookup.
     */
    void initialize() ;
    
    /** Method to shutdown the Karma Library's operations & free all allocated data..
     */
    void shutdown() ;
    
    /** Method to kick operation of the Karma library.
     */
    void start() ;
    
  private:
    
    /** Forward Declared structure to contain this object's data.
     */
    struct KarmaData* karma_data ;
    
    /** Method to retrieve a reference to this object's internal data structure.
     * @return Reference to this object's internal data structure.
     */
    KarmaData& data() ;
    
      /** Method to retrieve a const reference to this object's underlying data structure.
       * @return A const reference to this object's underlying data structure.
       */
    const KarmaData& data() const ;
};

#endif

