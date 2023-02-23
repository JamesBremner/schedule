class cJobType
{
public:
    cJobType()
    {

    }
    cJobType( const std::string& name, int crew )
        : myName( name)
        , myCrew( crew )
    {

    }
    std::string Name()
    {
        return myName;
    }
    int Crew()
    {
        return myCrew;
    }
    void CrewType( const std::vector< std::string >& type )
    {
        myCrewType = type;
    }
    std::vector< std::string >& CrewType()
    {
        return myCrewType;
    }
private:
    std::string myName;
    int myCrew;
    std::vector< std::string > myCrewType;
};

class cJob
{
public:
    cJob( const std::string& name,
          const std::string& type )
        : myType( type )
        , myName( name )
    {

    }
    std::string Type()
    {
        return myType;
    }
    void Type( const std::string& t )
    {
        myType = t;
    }
    std::string Name()
    {
        return myName;
    }
    void Name( const std::string n )
    {
        myName = n;
    }

private:
    std::string myType;
    std::string myName;
};

class cResourceType
{
public:
    cResourceType()
    {

    }
    cResourceType( const std::string& n )
        : myName( n )
    {

    }
    std::string Name()
    {
        return myName;
    }
private:
    std::string myName;
};

class cResource
{
public:
    cResource( const std::string& name,
               const std::string& type )
        : myName( name )
        , myType( type )
        , myfAssign( false )
        , myLastShift( -10 )
    {

    }
    std::string Name()
    {
        return myName;
    }
    void Name( const std::string& n )
    {
        myName = n;
    }
    std::string Type()
    {
        return myType;
    }
    void Type( const std::string& t )
    {
        myType = t;
    }
    std::string Text()
    {
        return myType + " " + myName;
    }

    void Assign( bool f, int shift = -1 );

    bool Assign() const
    {
        return myfAssign;
    }
    int LastShift() const
    {
        return myLastShift;
    }

    /** True if available for a shift
        @param[in] shift for potential assignment to
        @param[in] rotation minimum number of shifts between assignments + 1
    */
    bool IsAvailable( int shift, int rotation ) const;

private:
    std::string myName;
    std::string myType;
    bool myfAssign;
    int myLastShift;
};

class cFleet
{
public:
    cFleet( wex::gui& fm );
    void Read();
    void Write();

    void ResourceEditor();
    void JobEditor();
    void ConstraintEditor(wex::gui& fm);
    void Add( const cJob& v )
    {
        myJobVector.push_back( v );
    }

    void NewJobType();

    void NewResourceType( wex::gui& fm )
    {
        // nana::inputbox::text name("Name of type", "");

        // nana::inputbox inbox( fm, "New Person Type" );

        // if( inbox.show( name ) )
        // {
        //     cResourceType pt;
        //     int i;
        //     if( FindResourceType( name.value(), pt, i ))
        //     {
        //         nana::msgbox msg("Already have this resource type");
        //         msg.show();
        //     }
        //     else
        //     {
        //         myResourceTypeVector.push_back( cResourceType(
        //                                             name.value() ) );
        //     }
        // }
    }
    void NewJob();

    void NewResource();

    void Display();

    bool Schedule( int shifts );
    
    void JobTerm( const std::string& term )
    {
        myJobTerm = term;
    }
    std::string JobTerm() const
    {
        return myJobTerm;
    }
    void ResourceTerm( const std::string& term )
    {
        myResourceTerm = term;
    }
    std::string ResourceTerm() const
    {
        return myResourceTerm;
    }
    void Rotation( int r )
    {
        myShiftRotation = r;
    }
    void Test();


private:

    std::string myJobTerm;
    std::string myResourceTerm;
    int myShiftRotation;
    std::vector< cJob > myJobVector;
    std::vector< cJobType > myTypeVector;
    std::vector< cResource > myResourceVector;
    std::vector< cResourceType > myResourceTypeVector;
    std::vector< std::vector< std::pair< cJob, cResource > > > myAssign;
    wex::gui& myfm;
    wex::label& fleet_text;

    /** Search job types by name
        @param[in] type_name
        @param[out] type reference to job trype found
        @param[out] index of job type found
        @return true if job type found
    */
    bool FindType(
        const std::string& type_name,
        cJobType& type,
        int& index );

    bool FindResourceType(
        const std::string& name,
        cResourceType& type,
        int& index );

    bool FindPerson( const std::string& type )
    {
        for( auto& p : myResourceVector )
            if( p.Name() == type )
                return true;
        return false;
    }
};

