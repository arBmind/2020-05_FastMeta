Start-Process -NoNewWindow -FilePath $args[0] -Args $args[1..($args.length-1)] -Wait -PassThru | Select PeakWorkingSet64 | Write-Host
