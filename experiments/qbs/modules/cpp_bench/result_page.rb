require 'optparse'
require 'ostruct'
require 'erb'
require 'json'

options = OpenStruct.new
options.jsonFiles = []
options.templateFilePath = 'result_page.html.erb'
options.outputFilePath = 'output.html'
options.headline = 'Cpp Bench Results'
options.description = ''
options.dataVar = 'data'
options.showGroups = []
options.hideGroups = []

OptionParser.new do |opts|
  opts.banner = "Usage: buildResultpage.rb [options]"

  opts.on("-o", "--output FILEPATH", "File path where the HTML page should be saved to") do |out|
    options.outputFilePath = out
  end
  opts.on("-t", "--template FILEPATH", "File path of the ERB template") do |tpl|
    options.templateFilePath = tpl
  end
  opts.on("-h", "--headline TEXT", "Headline to give context") do |hdl|
    options.headline = hdl
  end
  opts.on("-d", "--description TEXT", "Description text to provide more details") do |dsc|
    options.description = dsc
  end
  opts.on("-v", "--dataVar TEXT", "Name of the data variable") do |dv|
    options.dataVar = dv
  end
  opts.on("--data x,y,z", Array, "List of json data files") do |data|
    options.jsonFiles += data
  end
  opts.on("--show x,y,z", Array, "List of data groups to show") do |groups|
    options.showGroups += groups
  end
  opts.on("--hide x,y,z", Array, "List of data groups to hide") do |groups|
    options.hideGroups += groups
  end
end.parse!(ARGV)

data = options.jsonFiles.map do |filePath|
  JSON.parse(File.read(filePath))
end

template = File.read(options.templateFilePath)
erb = ERB.new(template)
erb.filename = options.templateFilePath
html = erb.result(binding)
File.write(options.outputFilePath, html)
